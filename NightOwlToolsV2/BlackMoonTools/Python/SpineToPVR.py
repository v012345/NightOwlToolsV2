import os,shutil
from PIL import Image
from optparse import OptionParser
from FileCache import setCache,checkCache

res_default_path = "\\Resources\\res\\animation"
pvr_tool_cmd = "TexturePacker.exe %s --sheet %s.pvr.ccz --opt PVRTC4 --premultiply-alpha --padding 0 --no-trim --multipack --max-size 4096"
pvr_tool_path = "..\\TexturePacker"

def resizeImage(file_dir):
    if not os.path.exists(file_dir):
        os.makedirs(file_dir)
    base_img = Image.open(file_dir)
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
    out_img.save(file_dir)

def do_fixatlas(path):
    if os.path.exists(path):
        files = os.listdir(path)
        for i in files:
            if i.find(".atlas") != -1:
                # fix atlas file
                atlas_file_path = os.path.join(path, i)
                fp = open(atlas_file_path, 'r', encoding='utf-8')
                atlas_data = fp.read()
                fp.close()
                new_atlas_data = atlas_data.replace("png", "pvr.ccz")
                # fix image size
                lines = atlas_data.splitlines()

                path_index = 0
                while(lines[path_index].strip() == ""):#有的spine导出来的atlas首行是空的 然后才是文件名
                    path_index += 1

                png_file_path = os.path.join(path, lines[path_index])
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
                print("image name : %s"%(lines[path_index]))
                print("old image %s"%(lines[size_index]))
                lines[size_index] = lines[size_index].replace(height, str(max_size))
                lines[size_index] = lines[size_index].replace(width, str(max_size))
                print("new image %s"%(lines[size_index]))
                new_atlas_data = "\n".join(lines)

                fp = open(atlas_file_path, 'w', encoding='utf-8')
                fp.write(new_atlas_data)
                fp.close()
            else:
                new_path = os.path.join(path, i)
                if os.path.isdir(new_path):
                    do_fixatlas(new_path)

def do_spine2pvr(path):
    if os.path.exists(path):
        files = os.listdir(path)
        for i in files:
            if i.find(".png") != -1:
                # convert png to pvr
                png_file_path = os.path.join(path, i)
                if os.path.exists(png_file_path):
                    pvr_file_path = os.path.splitext(png_file_path)
                    print("=========>>>>>>>>> began convert to pvr :%s"%(png_file_path))

                    cache = checkCache(png_file_path,"_spine_pvr")
                    dist_path = pvr_file_path[0]+".pvr.ccz"
                    if not cache.exist:
                        resizeImage(png_file_path)
                        os.system(pvr_tool_cmd % (png_file_path, pvr_file_path[0]))
                        setCache(cache.cache, dist_path)
                    else:
                        shutil.copy(cache.cache,dist_path)
                    # delete png
                    os.remove(png_file_path)
                    #os.remove(os.path.join(path, "out.plist"))
            else:
                new_path = os.path.join(path, i)
                if os.path.isdir(new_path):
                    do_spine2pvr(new_path)

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option("-s", "--src",
                      action="store",
                      dest="srcPath",
                      type="string",
                      default=res_default_path,
                      help="set target srcPath."
                      )
    (opts, args) = parser.parse_args()
    res_path = opts.srcPath
    res_path = os.getcwd()+res_path
    os.chdir(pvr_tool_path)
    do_fixatlas(res_path)
    do_spine2pvr(res_path)

