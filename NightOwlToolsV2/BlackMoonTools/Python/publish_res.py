
import os, os.path
import re
import shutil
import hashlib
import datetime
import gzip,zipfile
from datetime import *
from optparse import OptionParser
import csv
import subprocess
import sys
import json

xxtea_cmd = "bin\\xxtea.exe e SMSP %s %s"
encrypt_cmd = "bin\\encrypt.exe \"%s\" \"%s\""
res_preprocessed = "\\res_preprocessed" # 加密前要预处理的资源, animation 与 plist 目录
res_encrypted = "\\res_encrypted" # 加密资源, 全部加密, 就算不加密也放进来, 因为出包要用, 不然资源太分散
res_compressed = "\\res_compressed" # 每个文件都被压缩成 gz 文件, 热更使用
need_create = [res_preprocessed,res_encrypted,res_compressed]
res2etc_cmd = "py -3 Python\\ResToEtc.py --from %s --to %s --res %s"
spine2pvr_cmd = "py -3 Python\\SpineToPvr.py"
plist2pvr_cmd = "py -3 Python\\PlistToPvr.py"
res2pvr_cmd = "py -3 Python\\PlistToPvr.py"
plist2etc_cmd = "py -3 Python\\PlistToEtc.py"
lua_exe = '..\\NightOwlToolsV2.exe'

def get_args():
    parser = OptionParser()
    parser.add_option(
        "-f", "--from_dir",
        action="store",
        dest="from_dir",
        default="",
        help="原始资源路径"
    )
    parser.add_option(
        "-t", "--to_dir",
        action="store",
        dest="to_dir",
        default="",
        help="处理好的资源放到哪里"
    )
    parser.add_option(
        "-p", "--platform",
        action="store",
        dest="platform",
        default="android",
        help="android 或者 ios"
    )
    parser.add_option(
        "-v", "--version",
        action="store",
        dest="version",
        default="",
        help="版本号, 目前的管理非常乱, 应该是要好几个人同时维护, 后来就没人维护了"
    )
    parser.add_option( 
        "--pack_all",
        action="store_true",
        dest="pack_all",
        default=False,
        help="默认只打差异文件, 带有这个参数就重新打所有文件"
    )
    parser.add_option(
        "-b", "--branch",
        action="store",
        dest="branch",
        default="trunk",
        help="打哪个分支的资源, 我感觉没有什么用, 不如直接给路径"
    )

    parser.add_option(
        "-r", "--res",
        action="store",
        dest="res",
        default="res",
        help="res, res_ch, 之类的"
    )
    return parser.parse_args()


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
    res_arr = opts.res.split(",")
    platform = opts.platform
    version = opts.version
    pack_all = opts.pack_all
    origin_dir = opts.from_dir
    # 所有资源输出目录的, 一般是个临时目录, 有点缓存的性质
    output_dir = opts.to_dir + "\\" +opts.branch + "\\" + opts.platform
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
 

    # 这里是看有多少文件要处理, 我想使用 lua 去管理, 思路就是去问 lua 要打哪些文件, 处理好了之后, 通知一下 lua 更新状态
    for p in need_create:
        subprocess.check_output([lua_exe, 'lua\\copyDirStruct.lua', origin_dir, output_dir+p],text=True)
    
    need_deal_files = "temp\\"+opts.branch+"_deal_files.txt"
    all_res = get_files_of_dir(origin_dir,need_deal_files)
   
    # 这四个要预处理
    to_preprocess = [
        p for p in all_res
        if (re.match(r'^/[^/]+/animation/.+\.atlas$', p) or
            re.match(r'^/[^/]+/animation/.+\.png$', p) or
            re.match(r'^/[^/]+/plist/.+\.plist$', p) or
            re.match(r'^/[^/]+/plist/.+\.png$', p))]
    
    # 打开文件写入
    need_preprocess_files = "temp\\"+opts.branch+"_preprocess_files.txt"
    with open(need_preprocess_files, "w") as f:
        for item in to_preprocess:
            f.write(item + "\n")
    preprocess_dir = output_dir+res_preprocessed
    cmd = res2pvr_cmd if platform == "ios" else res2etc_cmd
    # 处理完之后放到 res_preprocessed
    os.system(cmd % (origin_dir,preprocess_dir, need_preprocess_files))


    # 加密资源输出到
    encrypt_dir = output_dir+ res_encrypted
    dealed_files_path = "temp\\"+opts.branch+"_preprocessed_files.txt"
    dealed_files = get_files_of_dir(preprocess_dir,dealed_files_path)
    remain = list(set(all_res) - set(to_preprocess))
    
    i,tatol = 1,len(remain) + len(dealed_files)
    for f in dealed_files:
        encrypt_file(preprocess_dir+f,encrypt_dir+f)
        print(f"加密文件 : {i}/{tatol}", end='\r')
        i += 1
    for f in remain:
        encrypt_file(origin_dir+f,encrypt_dir+f)
        print(f"加密文件 : {i}/{tatol}", end='\r')
        i += 1
    print()
    compressed_dir = output_dir + res_compressed
    all_res = remain + dealed_files
    n, total = 1, len(all_res)
    for i in all_res:
        f = open(encrypt_dir + i, 'rb')
        c = f.read()
        f.close()
        g = gzip.GzipFile(
            filename='', mode='wb',
            compresslevel=9, mtime = 1,
            fileobj=open( compressed_dir+i+".gz", 'wb'))
        g.write(c)
        g.close()
        print(f"压缩文件 : {n}/{tatol}", end='\r')
        n += 1
    print()



    # 开始压缩, 热更用, 目前来看黑月的热更是使用最新版本的全部文件, 使用 md5 对比差值一个一个下载
    # 少量还行, 多的话太多下载连接了, 会很慢
    # dstdir = os.path.dirname(gzp)
    # if not os.path.exists(dstdir):
    #     os.makedirs(dstdir)

    # File.writegzip(enc, gzp, True)
 



