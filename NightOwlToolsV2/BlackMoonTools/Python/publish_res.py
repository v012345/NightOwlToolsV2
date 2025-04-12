
import os
import re
import shutil
import hashlib
import gzip
import csv
import subprocess
import optparse

xxtea_cmd = "bin\\xxtea.exe e SMSP %s %s"
encrypt_cmd = "bin\\encrypt.exe \"%s\" \"%s\""
platforms = ["ios", "android"]
preprocess_cmd = {
    "ios": "py -3 Python\\ResToPvr.py --from %s --to %s --res %s --out %s",
    "android": "py -3 Python\\ResToEtc.py --from %s --to %s --res %s --out %s"
}

lua_exe = '../NightOwlToolsV2.exe'


def get_args():
    parser = optparse.OptionParser()
    parser.add_option("-f", "--from_dir", action="store",
                      dest="from_dir", default="", help="原始资源路径")
    parser.add_option("-t", "--to_dir", action="store",
                      dest="to_dir", default="", help="处理好的资源放到哪里")
    return parser.parse_args()


def md5(filepath):
    mins = hashlib.md5()
    with open(filepath, 'rb') as fp:
        while chunk := fp.read(4096):
            mins.update(chunk)
    return mins.hexdigest()


def read_list(list_path, to_data):
    if os.path.isfile(list_path):
        with open(list_path, 'r') as f:
            for row in csv.reader(f):
                to_data[row[2]] = row


def get_files_of_dir(directory, outputTo):
    proc = subprocess.Popen([lua_exe, 'lua\\getFilesOfDir.lua', directory, outputTo],
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
    elif ext not in ['', '.mp3', '.xml', '.mp4', '.json']:
        os.system(encrypt_cmd % (src, to))
    else:
        shutil.copy(src, to)


if __name__ == "__main__":
    (opts, args) = get_args()
    origin_dir = opts.from_dir
    output_dir = opts.to_dir
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    print("调用 lua 复制游戏资源目录结构")
    subprocess.Popen([lua_exe, 'lua/copyDirStruct.lua', origin_dir, output_dir +
                     "/compressed_common"], creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    subprocess.Popen([lua_exe, 'lua/copyDirStruct.lua', origin_dir, output_dir +
                     "/encrypted_common"], creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    for p in platforms:
        for d in ["/preprocessed_", "/encrypted_", "/compressed_"]:
            subprocess.Popen([lua_exe, 'lua/copyDirStruct.lua', origin_dir,
                             output_dir+d+p], creationflags=subprocess.CREATE_NEW_CONSOLE).wait()

    print("调用 lua 获取需要更新资源")
    fDealFiles = output_dir+"/deal_files.txt"
    subprocess.Popen([lua_exe, 'lua\\getNeedDealFiles.lua', origin_dir, output_dir, fDealFiles],
                     creationflags=subprocess.CREATE_NEW_CONSOLE
                     ).wait()
    with open(fDealFiles, 'r', encoding='utf-8') as f:
        need_deal = [line.strip()
                     for line in f if line.strip()]  # 去掉空行和换行

    # 过滤出要预处理图集资源
    need_preprocess = [
        p for p in need_deal
        if (re.match(r'^/[^/]+/animation/.+\.atlas$', p) or
            re.match(r'^/[^/]+/animation/.+\.png$', p) or
            re.match(r'^/[^/]+/plist/.+\.plist$', p) or
            re.match(r'^/[^/]+/plist/.+\.png$', p))]
    common = list(set(need_deal) - set(need_preprocess))

    # 加密文件
    i, tatol = 0, len(common)
    print(f"加密共有资源 : {i}/{tatol}", end='\r')
    if tatol > 0:
        for f in common:
            encrypt_file(origin_dir+f, output_dir+"/encrypted_common"+f)
            i += 1
            print(f"加密共有资源 : {i}/{tatol}", end='\r')
    print()

    i, tatol = 0, len(common)
    print(f"压缩共有的已加密资源 : {i}/{tatol}", end='\r')
    for f in common:
        with open(output_dir+"/encrypted_common" + f, 'rb') as t:
            c = t.read()
        with gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime=1, fileobj=open(output_dir+"/compressed_common" + f + ".gz", 'wb')) as g:
            g.write(c)
        i += 1
        print(f"压缩共有的已加密资源 : {i}/{tatol}", end='\r')
    print()

    common_list_v1, common_list_v2 = {}, {}
    i, tatol = 0, len(common)
    print(f"计算共有文件签名 : {i}/{tatol}", end='\r')
    for f in common:
        encrypt_res_md5 = md5(output_dir+"/encrypted_common"+f)
        gz_file_name = output_dir+"/compressed_common"+f+".gz"
        compressed_res_md5 = md5(gz_file_name)
        key = f.lstrip('/')+".gz"
        size = os.path.getsize(gz_file_name)
        common_list_v1[key] = [encrypt_res_md5, size, key]
        common_list_v2[key] = [encrypt_res_md5, size, key, compressed_res_md5]
        i += 1
        print(f"计算共有文件签名 : {i}/{tatol}", end='\r')
    print()

    # 打开文件写入
    preprocess_files = output_dir+"/preprocess_files.txt"
    with open(preprocess_files, "w") as f:
        for item in need_preprocess:
            f.write(item + "\n")

    # 处理完之后放到 res_preprocessed
    for p in platforms:
        preprocessed_files = output_dir + "/"+p+"_preprocessed_files.txt"
        os.system(preprocess_cmd[p] % (
            origin_dir, output_dir+"/preprocessed_"+p, preprocess_files, preprocessed_files))

        # 加密处理出来的文件
        with open(preprocessed_files, 'r', encoding='utf-8') as f:
            dealed_files = [line.strip() for line in f if line.strip()]

        i, tatol = 0, len(dealed_files)
        print(f"加密 {p} 独有资源 : {i}/{tatol}", end='\r')
        for f in dealed_files:
            encrypt_file(output_dir+"/preprocessed_"+p +
                         f, output_dir+"/encrypted_"+p+f)
            i += 1
            print(f"加密 {p} 独有资源 : {i}/{tatol}", end='\r')
        print()

        # 压缩加密的文件
        i, tatol = 0, len(dealed_files)
        print(f"压缩 {p} 独有的已加密资源 : {i}/{tatol}", end='\r')
        for f in dealed_files:
            with open(output_dir+"/encrypted_"+p+f, 'rb') as t:
                c = t.read()
            with gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime=1, fileobj=open(output_dir+"/compressed_"+p+f + ".gz", 'wb')) as g:
                g.write(c)
            i += 1
            print(f"压缩 {p} 独有的已加密资源 : {i}/{tatol}", end='\r')
        print()

        # 生成对比文件, v1 和 v2 只是热更程序升级了 v2 版, 但是为了兼容 v1 版, 还要生成 v1 的 list
        list_v1, list_v2 = {}, {}
        read_list(output_dir+"/encrypted_"+p+"/list.csv", list_v1)
        read_list(output_dir+"/encrypted_"+p+"/list-v2.csv", list_v2)

        i, tatol = 0, len(dealed_files)
        print(f"计算 {p} 资源的签名 : {i}/{tatol}", end='\r')
        for f in dealed_files:
            encrypt_res_md5 = md5(output_dir+"/encrypted_"+p+f)
            gz_file_name = output_dir+"/compressed_"+p+f+".gz"
            compressed_res_md5 = md5(gz_file_name)
            key = f.lstrip('/')+".gz"
            size = os.path.getsize(gz_file_name)
            list_v1[key] = [encrypt_res_md5, size, key]
            list_v2[key] = [encrypt_res_md5, size, key, compressed_res_md5]
            i += 1
            print(f"计算 {p} 资源的签名 : {i}/{tatol}", end='\r')
        print()

        for i in [{"data": list_v1 | common_list_v1, "path": "/list.csv"}, {"data": list_v2 | common_list_v2, "path": "/list-v2.csv"}]:
            data = i["data"]
            with open(output_dir+"/encrypted_"+p + i["path"], 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                for v in data.values():
                    writer.writerow(v)
            with open(output_dir+"/encrypted_"+p + i["path"], 'rb') as f:
                c = f.read()
            with gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime=1, fileobj=open(output_dir+"/compressed_"+p + i["path"] + ".gz", 'wb')) as g:
                g.write(c)
    # 完成之后 更新 lua 的文件状态数据库
    subprocess.Popen([lua_exe, 'lua\\updateFileStateDB.lua', origin_dir, output_dir, fDealFiles],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
