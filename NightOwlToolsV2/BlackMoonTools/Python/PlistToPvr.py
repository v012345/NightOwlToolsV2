import os,shutil
from PIL import Image
from optparse import OptionParser
from FileCache import setCache,checkCache

res_default_path = "\\Resources\\res\\plist"
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


def do_plist2pvr(path):
    if os.path.exists(path):
        files = os.listdir(path)
        for i in files:
            if i.find(".plist") != -1:
                # fix plist file
                plist_file_path = os.path.join(path, i)
                fp = open(plist_file_path, 'r', encoding='utf-8')
                data = fp.read()
                fp.close()
                png = os.path.splitext(i)[0] + ".png"
                pvr = os.path.splitext(i)[0] + ".pvr.ccz"
                data = data.replace(png,pvr)
                png_file_path = os.path.join(path, png)
                fp = open(png_file_path, 'rb')
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
                fp = open(plist_file_path, 'w', encoding='utf-8')
                fp.write(data)
                fp.close()
                print("=========>>>>>>>>> finish fix plist file :%s"%(plist_file_path))
            elif i.find(".png") != -1:
                # convert png to pvr
                png_file_path = os.path.join(path, i)
                if os.path.exists(png_file_path):
                    pvr_file_path = os.path.splitext(png_file_path)
                    print("=========>>>>>>>>> began convert to pvr :%s"%(png_file_path))
                    cache = checkCache(png_file_path,"_plist_pvr")
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
                    do_plist2pvr(new_path)

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
    print(res_path)
    do_plist2pvr(res_path)

