
import os, re, shutil, hashlib, gzip,csv,subprocess,optparse

xxtea_cmd = "bin\\xxtea.exe e SMSP %s %s"
encrypt_cmd = "bin\\encrypt.exe \"%s\" \"%s\""
platforms = ["ios","android"]
preprocess_cmd = {
    "ios":"py -3 Python\\ResToPvr.py --from %s --to %s --res %s",
    "android":"py -3 Python\\ResToEtc.py --from %s --to %s --res %s"
}

lua_exe = '../NightOwlToolsV2.exe'

def get_args():
    parser = optparse.OptionParser()
    parser.add_option( "-f", "--from_dir", action="store", dest="from_dir", default="", help="原始资源路径" )
    parser.add_option( "-t", "--to_dir", action="store", dest="to_dir", default="", help="处理好的资源放到哪里" )
    parser.add_option( "-p", "--platform", action="store", dest="platform", default="android", help="android 或者 ios" )
    parser.add_option( "-v", "--version", action="store", dest="version", default="", help="版本号, 目前的管理非常乱, 应该是要好几个人同时维护, 后来就没人维护了")
    parser.add_option( "--pack_all", action="store_true", dest="pack_all", default=False, help="默认只打差异文件, 带有这个参数就重新打所有文件")
    parser.add_option( "-b", "--branch", action="store", dest="branch", default="trunk", help="打哪个分支的资源, 我感觉没有什么用, 不如直接给路径")
    parser.add_option( "-r", "--res", action="store", dest="res", default="res", help="res, res_ch, 之类的" )
    return parser.parse_args()

def md5(filepath):
    mins = hashlib.md5()
    with open(filepath, 'rb') as fp:
        while chunk := fp.read(4096):
            mins.update(chunk)
    return mins.hexdigest()

def read_list(list_path,to_data):
    if os.path.isfile(list_path):
        with open(list_path, 'r') as f:
            for row in csv.reader(f):
                to_data[row[2]] = row

def get_files_of_dir(directory,outputTo ):
    proc = subprocess.Popen([lua_exe, 'lua\\getFilesOfDir.lua', directory,outputTo],
        creationflags=subprocess.CREATE_NEW_CONSOLE)
    proc.wait()  # 等待新窗口中的 Lua 执行完成
    # 读取路径列表文件
    with open(outputTo, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f if line.strip()]  # 去掉空行和换行
    return lines

def encrypt_file(src, to):
    _, ext = os.path.splitext(src)
    if ext == '.lua':
        os.system(xxtea_cmd % (src, to))
    elif ext not in ['','.mp3', '.xml', '.mp4', '.json']:
        os.system(encrypt_cmd %(src,to))
    else:
        shutil.copy(src, to)
    

if __name__ == "__main__":
    (opts, args) = get_args()
    pack_all = opts.pack_all
    origin_dir = opts.from_dir
    # 所有资源输出目录的, 一般是个临时目录, 有点缓存的性质
    output_dir = opts.to_dir + "\\" +opts.branch
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
   

    # 这里是看有多少文件要处理, 我想使用 lua 去管理, 思路就是去问 lua 要打哪些文件, 处理好了之后, 通知一下 lua 更新状态
    print("调用 lua 复制游戏资源目录结构")
    subprocess.Popen([lua_exe, 'lua/copyDirStruct.lua', origin_dir, output_dir+"/res_compressed_common"],creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    subprocess.Popen([lua_exe, 'lua/copyDirStruct.lua', origin_dir, output_dir+"/res_encrypted_common"],creationflags=subprocess.CREATE_NEW_CONSOLE).wait() 
    for p in platforms:
        for d in ["/res_preprocessed","/res_encrypted","/res_compressed"]:
            subprocess.Popen([lua_exe, 'lua/copyDirStruct.lua', origin_dir, output_dir+d+"_"+p],creationflags=subprocess.CREATE_NEW_CONSOLE).wait() 
    
    print("调用 lua 获取需要更新资源")
    deal_files = output_dir+"\\"+opts.branch+"_deal_files.txt"
    all_res = get_files_of_dir(origin_dir,deal_files)

    # 过滤出要预处理图集资源
    need_preprocess = [
        p for p in all_res
        if (re.match(r'^/[^/]+/animation/.+\.atlas$', p) or
            re.match(r'^/[^/]+/animation/.+\.png$', p) or
            re.match(r'^/[^/]+/plist/.+\.plist$', p) or
            re.match(r'^/[^/]+/plist/.+\.png$', p))]
    common = list(set(all_res) - set(need_preprocess))

    # 打开文件写入
    need_preprocess_files = output_dir+"\\"+opts.branch+"_preprocess_files.txt"
    with open(need_preprocess_files, "w") as f:
        for item in need_preprocess:
            f.write(item + "\n")
    


    # 处理完之后放到 res_preprocessed
    for p in platforms:
        os.system(preprocess_cmd[p] % (origin_dir,output_dir+"/res_preprocessed_"+p, need_preprocess_files))


    # 加密文件
    i,tatol = 1,len(common)
    if tatol > 0:
        for f in common:
            encrypt_file(origin_dir+f,output_dir+"/res_encrypted_common"+f)
            print(f"加密共有资源 : {i}/{tatol}", end='\r')
            i += 1
    else:
         print(f"加密共有资源 : 0/0", end='\r')
    print()

    for p in platforms:
        preprocess_files_path = output_dir +"/"+opts.branch+"_"+p+"_preprocessed_files.txt"
        dealed_files = get_files_of_dir(output_dir+"/res_preprocessed_"+p,preprocess_files_path)
        i,tatol = 1,len(dealed_files)
        for f in dealed_files:
            encrypt_file(output_dir+"/res_preprocessed_"+p+f,output_dir+"/res_encrypted_"+p+f)
            print(f"加密 {p} 独有资源 : {i}/{tatol}", end='\r')
            i += 1
        print()


    i,tatol = 1,len(common)
    for f in common:
        with open(output_dir+"/res_encrypted_common" + f, 'rb') as t:
                c = t.read()
        with gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime=1, fileobj=open(output_dir+"/res_compressed_common" + f + ".gz", 'wb')) as g:
                g.write(c)
        print(f"压缩共有的已加密资源 : {i}/{tatol}", end='\r')
        i += 1
    print()
    for p in platforms:
        encrypted_files_path = output_dir +"/"+opts.branch+"_"+p+"_encrypted_files.txt"
        encrypted_files = get_files_of_dir(output_dir+"/res_encrypted_"+p,encrypted_files_path)
        i,tatol = 1,len(encrypted_files)
        for f in encrypted_files:
            with open(output_dir+"/res_encrypted_"+p+f, 'rb') as t:
                c = t.read()
            with gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime=1, fileobj=open(output_dir+"/res_compressed_"+p+f + ".gz", 'wb')) as g:
                    g.write(c)
            print(f"压缩 {p} 独有的已加密资源 : {i}/{tatol}", end='\r')
            i += 1
        print()




    common_list_v1,common_list_v2 = {},{}
    for f in common:
        encrypt_res_md5 = md5(output_dir+"/res_encrypted_common"+f)
        gz_file_name = output_dir+"/res_compressed_common"+f+".gz"
        compressed_res_md5 = md5(gz_file_name)
        key = f.lstrip('/')+".gz"
        size = os.path.getsize(gz_file_name)
        common_list_v1[key] = [encrypt_res_md5,size,key]
        common_list_v2[key] = [encrypt_res_md5,size,key,compressed_res_md5]
        print(f"计算共有文件签名 : {i}/{tatol}", end='\r')
        i += 1
    print()


    for p in platforms:
        # 生成对比文件, v1 和 v2 只是热更程序升级了 v2 版, 但是为了兼容 v1 版, 还要生成 v1 的 list
        list_v1,list_v2 = {},{}
        read_list(output_dir+"/res_encrypted_"+p+"/list.csv", list_v1)
        read_list(output_dir+"/res_encrypted_"+p+"/list-v2.csv", list_v2)
        with open( output_dir +"/"+opts.branch+"_"+p+"_encrypted_files.txt", 'r', encoding='utf-8') as f:
            encrypted_files = [line.strip() for line in f if line.strip()]  # 去掉空行和换行

        i,tatol = 1,len(encrypted_files)
        for f in encrypted_files:
            encrypt_res_md5 = md5(output_dir+"/res_encrypted_"+p+f)
            gz_file_name = output_dir+"/res_compressed_"+p+f+".gz"
            compressed_res_md5 = md5(gz_file_name)
            key = f.lstrip('/')+".gz"
            size = os.path.getsize(gz_file_name)
            list_v1[key] = [encrypt_res_md5,size,key]
            list_v2[key] = [encrypt_res_md5,size,key,compressed_res_md5]
            print(f"计算 {p} 资源的签名 : {i}/{tatol}", end='\r')
            i += 1
        print()
        for i in [{"data": list_v1|common_list_v1, "path": "/list.csv"}, {"data": list_v2|common_list_v2, "path": "/list-v2.csv"}]:
            data = i["data"] 
            with open(output_dir+"/res_encrypted_"+p + i["path"], 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                for v in data.values():
                    writer.writerow(v)
            with open(output_dir+"/res_encrypted_"+p + i["path"], 'rb') as f:
                c = f.read()
            with gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime=1, fileobj=open(output_dir+"/res_compressed_"+p + i["path"] + ".gz", 'wb')) as g:
                g.write(c)

 



