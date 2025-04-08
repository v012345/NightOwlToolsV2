import os
from optparse import OptionParser
# etc_tool_cmd = 'etcpack.exe "%s" "%s" -v -c etc1 -as -s fast >>nul 2>&1' # 还是要错的
etc_tool_cmd = 'etcpack.exe "%s" "%s" -v -c etc1 -as -s fast >>nul'

def replace_in_file(src, target, old, new):
    with open(src, 'r', encoding='utf-8') as f:
        data = f.read().replace(old, new)
    with open(target, 'w', encoding='utf-8') as f:
        f.write(data)

def do_spine2etc(src, to , res):
     # 读取路径列表文件
    with open(res, 'r', encoding='utf-8') as f:
        all_res = [line.strip() for line in f if line.strip()]  # 去掉空行和换行
    os.chdir("bin") # 不影响上层, 还好, 为什么要进行, 因为 etcpack.exe 这个工具只能在自己目录下跑, 好像是要生成文件
    n,tatol = 1,len(all_res)
    for i in all_res:
        s,t = src + i,to + i
        if i.endswith(".atlas") :
            replace_in_file(s,t, "png", "pkm")
        elif i.endswith(".plist") :
            file_name = os.path.splitext(os.path.basename(i))[0]
            png = file_name + ".png"
            pkm = file_name + ".pkm"
            replace_in_file(s,t, png, pkm)
        elif i.endswith(".png") :
            os.system(etc_tool_cmd % (s, os.path.dirname(t)))
            pkm = os.path.splitext(t)[0]
            os.replace(pkm + "_alpha.pkm", pkm + ".pkm@alpha")
        print(f"正在转为etc : {n}/{tatol}", end='\r')
        n += 1
    print() 



if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option("-f", "--from", action="store", dest="src", type="string", help="要处理的文件目录")
    parser.add_option("-t", "--to", action="store", dest="to", type="string", help="输出到的目录")
    parser.add_option("-r", "--res", action="store", dest="res", type="string", help="资源")
    (opts, args) = parser.parse_args()
    do_spine2etc(opts.src,opts.to,opts.res)


