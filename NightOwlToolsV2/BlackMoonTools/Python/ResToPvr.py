import os,shutil
try:
    from PIL import Image
except:
    import subprocess, sys
    subprocess.Popen([sys.executable, "-m", "pip", "install", "pillow"],
        creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
from optparse import OptionParser
# pvr_tool_cmd = 'TexturePacker.exe "%s" --sheet "%s" --opt PVRTC4 --premultiply-alpha --padding 0 --no-trim --multipack --max-size 4096 >>nul 2>&1'
pvr_tool_cmd = 'TexturePacker.exe "%s" --sheet "%s" --opt PVRTC4 --premultiply-alpha --padding 0 --no-trim --multipack --max-size 4096 >>nul'
# def replace_in_file(src, target, old, new):
#     with open(src, 'r', encoding='utf-8') as f:
#         data = f.read().replace(old, new) # 会修改换行符, 很尴尬
#     with open(target, 'w', encoding='utf-8') as f:
#         f.write(data)

def adjustPngTo(png,to):
    base_img = Image.open(png)
    img_size = base_img.size
    max_size = max(img_size[0], img_size[1])
    i = 2
    while True:
        if max_size <= 2**i:
            max_size = 2**i
            break
        i = i + 1
    out_img = Image.new("RGBA",size = (max_size,max_size))
    out_img.paste(base_img, (0, 0, img_size[0], img_size[1]))
    out_img.save(to)

def resToPvr(src, to , res):
     # 读取路径列表文件
    with open(res, 'r', encoding='utf-8') as f:
        all_res = [line.strip() for line in f if line.strip()]  # 去掉空行和换行
    os.chdir("bin/TexturePacker") # 不影响上层, 还好, 为什么要进行, 因为 etcpack.exe 这个工具只能在自己目录下跑, 好像是要生成文件
    n,tatol = 1,len(all_res)
    for i in all_res:
        s,t = src + i,to + i
        if i.endswith(".atlas") :
            fp = open(s, 'r', encoding='utf-8')
            atlas_data = fp.read()
            fp.close()
            new_atlas_data = atlas_data.replace("png", "pvr.ccz")
            # fix image size
            lines = atlas_data.splitlines()
            path_index = 0
            while(lines[path_index].strip() == ""):#有的spine导出来的atlas首行是空的 然后才是文件名
                path_index += 1

            png_file_path = os.path.join( os.path.dirname(s), lines[path_index])
            fp = open(png_file_path, 'rb')
            base_img = Image.open(fp)
            img_size = base_img.size
            fp.close()
            max_size = max(img_size[0], img_size[1])
            i = 2
            while True:
                if max_size <= 2**i:
                    max_size = 2**i
                    break
                i = i + 1

            size_index = path_index + 1
            while(not lines[size_index].startswith("size:")):
                size_index += 1

            lines = new_atlas_data.splitlines()
            width = lines[size_index].split(",")[0].split()[1]
            height = lines[size_index].split(",")[1]
            lines[size_index] = lines[size_index].replace(height, str(max_size))
            lines[size_index] = lines[size_index].replace(width, str(max_size))
            new_atlas_data = "\n".join(lines)

            fp = open(t, 'w', encoding='utf-8')
            fp.write(new_atlas_data)
            fp.close()

        elif i.endswith(".plist") :
            file_name = os.path.splitext(os.path.basename(i))[0]
            fp = open(s, 'r', encoding='utf-8')
            data = fp.read()
            fp.close()
            png = os.path.splitext(file_name)[0] + ".png"
            pvr = os.path.splitext(file_name)[0] + ".pvr.ccz"
            data = data.replace(png,pvr)
            
            fp = open(src + os.path.splitext(i)[0] + ".png" , 'rb')
            base_img = Image.open(fp)
            img_size = base_img.size
            fp.close()
            old_size_string = "{%d,%d}"%(img_size[0],img_size[1])
            max_size = max(img_size[0], img_size[1])
            i = 2
            while True:
                if max_size <= 2**i:
                    max_size = 2**i
                    break
                i = i + 1
            new_size_string = "{%d,%d}"%(max_size,max_size)
            data = data.replace(">%d<"%(img_size[0]),">%d<"%(max_size))
            data = data.replace(">%d<"%(img_size[1]),">%d<"%(max_size))
            data = data.replace(old_size_string,new_size_string)
            fp = open(t, 'w', encoding='utf-8')
            fp.write(data)
            fp.close()
        elif i.endswith(".png") :
            pvr_file_path = os.path.splitext(t)
            adjustPngTo(png_file_path,pvr_file_path[0]+".png")
            os.system(pvr_tool_cmd % (pvr_file_path[0]+".png", pvr_file_path[0]+".pvr.ccz"))
            os.remove(pvr_file_path[0]+".png")
        print(f"正在转为pvr : {n}/{tatol}", end='\r')
        n += 1
    print() 


#   --from C:\Users\Meteor\Desktop\temp\Resources --to C:\Users\Meteor\Desktop\temp\yougubt5\Resources\yougubt5/res_preprocessed_ios --res C:\Users\Meteor\Desktop\temp\yougubt5\Resources\yougubt5\yougubt5_preprocess_files.txt
if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option("-f", "--from", action="store", dest="src", type="string", help="要处理的文件目录")
    parser.add_option("-t", "--to", action="store", dest="to", type="string", help="输出到的目录")
    parser.add_option("-r", "--res", action="store", dest="res", type="string", help="资源")
    (opts, args) = parser.parse_args()
    resToPvr(opts.src,opts.to,opts.res)


