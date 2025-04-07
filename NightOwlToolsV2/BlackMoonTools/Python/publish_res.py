
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
        default=branch_default,
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


class File:
    @staticmethod
    def md5(filepath, keydir):
        content = File.read(filepath)
        mins = hashlib.md5()
        content = content.decode("utf-8", "ignore") + keydir
        mins.update(content.encode())
        text = mins.hexdigest()
        return str.upper(text)

    @staticmethod
    def md5ext(filepath, ext):
        content = File.read(filepath)
        content =content.decode() + ext
        mins = hashlib.md5()
        mins.update(content.encode())
        text = mins.hexdigest()
        return str.upper(text)
    @staticmethod
    def md5gzp(filepath):
        content = File.read(filepath)
        mins = hashlib.md5()
        mins.update(content)
        text = mins.hexdigest()
        return text

    @staticmethod
    def read(filepath):
        try:
            fp = open(filepath, 'rb')
            content = fp.read()
            fp.close()
        except:
            return None
        finally:
            return content

    @staticmethod
    def write(filepath, content, mode="wb+"):
        try:
            fp = open(filepath, mode)
            fp.write(content)
            fp.close()
        except:
            pass
        finally:
            pass

    @staticmethod
    def writegzip(src, dst, path=False):
        dstdir = os.path.dirname(dst)
        if not os.path.exists(dstdir):
            os.makedirs(dstdir)

        g = gzip.GzipFile(filename='', mode='wb', compresslevel=9, mtime = "1", fileobj=open(dst, 'wb'))
        if path:
            g.write(File.read(src))
        else:
            g.write(src.encode())
        g.close()

    @staticmethod
    def ergodic(dirpaths):
        if isinstance(dirpaths, str):
            dirpaths = [dirpaths]

        flist = []
        for dirpath in dirpaths:
            dpath = os.path.realpath(dirpath)
            if not os.path.isdir(dpath):
                print('Path [%s] is not a directory!' % dpath)
                continue

            for root, dirs, files in os.walk(dpath):
                for file in files:
                    fpath = os.path.join(root, file)
                    flist.append(fpath)
        return flist

    @staticmethod
    def zipDir(dirpath, outFullName):
        """
        压缩指定文件夹
        :param dirpath: 目标文件夹路径
        :param outFullName: 压缩文件保存路径+xxxx.zip
        :return: 无
        """
        # zip = zipfile.ZipFile(outFullName,"w",zipfile.ZIP_DEFLATED)
        # for path,dirnames,filenames in os.walk(dirpath):
        #     # 去掉目标跟路径，只对目标文件夹下边的文件及文件夹进行压缩
        #     fpath = path.replace(dirpath,'')

        #     for filename in filenames:
        #         zip.write(os.path.join(path,filename),os.path.join(fpath,filename))
        # zip.close()

        command = [
            "C:\\Program Files\\7-Zip\\7z.exe", 
            "a",
            outFullName, 
            dirpath + os.sep + "*"
        ]
        print("zip: "+dirpath+" -> "+outFullName)
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        if result.returncode != 0:
            print("zip invalid ret: "+str(result.returncode))
            sys.exit(-1)

class Filter:
    def __init__(self):
        self.irl = []  # include regex list
        self.erl = []  # exclude regex list

    def cleanup(self):
        self.irl = []
        self.erl = []

    def addir(self, regex_str):
        r = re.compile(regex_str, re.I)
        self.irl.append(r)
        return self

    def adder(self, regex_str):
        r = re.compile(regex_str, re.I)
        self.erl.append(r)
        return self

    def _isinc(self, s):
        for r in self.irl:
            if r.search(s) > 0:
                return True
        return False

    def _isexc(self, s):
        for r in self.erl:
            if r.search(s) != None:
                return True
        return False

    def check(self, fpath):
        if self._isexc(fpath):
            return False
        if self._isinc(fpath):
            return True
        return True

    def filter(self, slist):
        if isinstance(slist, str):
            slist = [slist]

        rlist = []
        for s in slist:
            if self.check(s):
                rlist.append(s)
        return rlist


class info:
    def __init__(self, src, usetime=False):
        self.src = src
        self.enc = src.replace(res_copy_path, encrypt_path)
        self.gzp = src.replace(res_copy_path, publish_path) + ".gz"
        self.key = self.gzp.split(publish_path + "\\")[1]
        self.key = self.key.replace("\\", '/')

        # print("src:%s"% self.src)
        # print("enc:%s"% self.enc)
        # print("gzp:%s"% self.gzp)
        # print("key:%s"% self.key)
        _, self.ext = os.path.splitext(src)

        self.usetime = usetime




from_dir = ""
language_dir_sep = "-"
xxtea_cmd = "xxtea e SMSP %s %s"
luajit_cmd = "..\\luajit\\luajit -b %s %s"
encrypt_cmd = "encrypt"
trunk_src_path = "..\\..\\trunk\\Resources"
other_src_path = "..\\..\\branches\\%s\\Resources"
need_paths = ["encryptRes","publishRes","Resources"]
spine2etc_cmd = "py -3 Python\\SpineToEtc.py"
spine2pvr_cmd = "py -3 Python\\SpineToPvr.py"
plist2pvr_cmd = "py -3 Python\\PlistToPvr.py"
plist2etc_cmd = "py -3 Python\\PlistToEtc.py"

branch_default = "trunk"
version = ""



if __name__ == "__main__":
    (opts, args) = get_args()
    res_arr = opts.res.split(",")
    platform = opts.platform
    version = opts.version
    pack_all = opts.pack_all
    branch_path = "\\" + opts.branch
    platform_path = "\\" + opts.platform


    # 资源输出目录
    output_dir = opts.to_dir + "\\" +opts.branch + "\\" + opts.platform
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 这里有一步是看有多少文件要加密, 我想使用 lua 去管理, 思路就是去问 lua 要打哪些文件, 打好了, 通知一下 lua
    # 把原始的资源复制过来, 因为有的工具会直接改还文件, 而且遍历文件夹
    for p in need_paths:
        subprocess.check_output(['..\\NightOwlToolsV2.exe', 'lua\\copyDirStruct.lua', opts.from_dir, output_dir+"\\"+p],text=True)
    deal_files_path = "temp\\"+opts.branch+"_deal_files.txt"
    proc = subprocess.Popen(
        ['..\\NightOwlToolsV2.exe', 'lua\\getDealFiles.lua', opts.from_dir, output_dir+"\\"+"Resources",deal_files_path ],
        creationflags=subprocess.CREATE_NEW_CONSOLE)
    proc.wait()  # 等待新窗口中的 Lua 执行完成

    


    # 读取路径列表文件
    with open(deal_files_path, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f if line.strip()]  # 去掉空行和换行

    all_paths = lines
    atlas_files, plist_files,png_paths = [],[],[]
    for f in res_arr:
        atlas_files += [p for p in all_paths if p.startswith('/%s/animation'%f) and p.endswith('.atlas')]
        plist_files += [p for p in all_paths if p.startswith('/%s/plist'%f) and p.endswith('.plist')]
        png_paths += [p for p in all_paths if (p.startswith('/%s/animation'%f) or p.startswith('/%s/plist'%f)) and p.endswith('.png')]


    print(all_paths)
    print(atlas_files)
    print(plist_files)
    print(png_paths)
   
    # 加密资源输出到
    encrypt_res_dir = output_dir+ "\\" + "encryptRes"
    print(encrypt_res_dir)
 
    is_ios = platform == "ios"
    deal_spine_cmd = spine2pvr_cmd if is_ios else spine2etc_cmd
    deal_plist_cmd = plist2pvr_cmd if is_ios else spine2etc_cmd # 安卓两个文件合并了
    for f in res_arr:
        os.system(deal_spine_cmd + " -s %s\\Resources\\%s\\animation" % (output_dir,f))
        os.system(deal_plist_cmd + " -s %s\\Resources\\%s\\plist" % (output_dir,f))


    exit(0)
    for f in all_paths:
        if ext == '.lua':
            os.system(xxtea_cmd % (enc, enc))
        elif len(ext) > 1 and ext not in ['.mp3', '.xml', '.mp4', '.json']:
            shutil.copy2(src, enc)
            os.system(encrypt_cmd + " " + enc)

    


    # 开始
    # dstdir = os.path.dirname(gzp)
    # if not os.path.exists(dstdir):
    #     os.makedirs(dstdir)

    # File.writegzip(enc, gzp, True)
 



