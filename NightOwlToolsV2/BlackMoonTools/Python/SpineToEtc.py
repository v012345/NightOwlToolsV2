import os
from optparse import OptionParser
etc_tool_cmd = 'etcpack.exe "%s" "%s" -v -c etc1 -as -s fast'

def replace_in_file(path, old, new):
    with open(path, 'r', encoding='utf-8') as f:
        data = f.read().replace(old, new)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(data)

def do_spine2etc(path):
    files = os.listdir(path)
    for i in files:
        if i.find(".atlas") != -1:
            # fix atlas file
            atlas = os.path.join(path, i)
            replace_in_file(atlas, "png", "pkm")
        elif i.find(".plist") != -1:
            # fix atlas file
            plist = os.path.join(path, i)
            png = os.path.splitext(i)[0] + ".png"
            pkm = os.path.splitext(i)[0] + ".pkm"
            replace_in_file(plist, png, pkm)
        elif i.find(".png") != -1:
            # convert png to etc
            png = os.path.join(path, i)
            if os.path.exists(png):
                os.system(etc_tool_cmd % (png, path))
                os.remove(png)
                pkm = os.path.splitext(png)[0]
                os.rename(pkm + "_alpha.pkm", pkm + ".pkm@alpha")
        else:
            sub_path = os.path.join(path, i)
            if os.path.isdir(sub_path):
                do_spine2etc(sub_path)

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option("-s", "--src", action="store", dest="srcPath", type="string", help="要处理的文件目录")
    (opts, args) = parser.parse_args()
    if os.path.exists(opts.srcPath):
        os.chdir("bin") # 不影响上层, 还好, 为什么要进行, 因为 etcpack.exe 这个工具只能在自己目录下跑, 好像是要生成文件
        do_spine2etc(opts.srcPath)


