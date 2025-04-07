import os
import re
import sys
import optparse

SVN_EMPTY_VER = 0
pathSaveGameVersion: str = None
checkFolder: str = None
pathRoot:str = None
PATH_GAME_RES_VERSION_LUA = "src/imports/GameResVersion.lua"


def system(cmd: str):
    print("system: " + cmd)
    return os.system(cmd)


def popen(cmd: str):
    print("popen: " + cmd)
    return os.popen(cmd)


def Update(file: str):
    system("svn update " + file)


def Reset(file: str):
    system("svn revert --recursive \"%s\"" % file)  # 恢复svn添加的svn文件

def GetSvnVersion(file: str):
    f = popen("svn log -r HEAD:1 --limit 1 %s" % file)
    lines = f.read().strip().split("\n")
    f.close()
    for line in lines:
        find_list = re.findall(r"^r(\d+)\s*\|", line)
        if len(find_list) > 0:
            return find_list[0]
    return SVN_EMPTY_VER


def ResetVersion():
    if os.path.exists(pathSaveGameVersion):  # 存在才还原版本号
        # Reset(pathSaveGameVersion)
        json_f = open(pathSaveGameVersion, "w", encoding="utf-8")
        json_f.write("GameResVersion=1")
        json_f.close()


def WriteVersion():
    if os.path.exists(pathSaveGameVersion):  # 存在才写入版本号
        newVersion = GetSvnVersion(pathRoot)
        if newVersion == SVN_EMPTY_VER:
            sys.exit(-3)
        else:
            print("GameResVersion:"+str(newVersion))
            json_f = open(pathSaveGameVersion, "w", encoding="utf-8")
            json_f.write("GameResVersion=" + newVersion)
            json_f.close()


if __name__ == "__main__":
    parser = optparse.OptionParser()
    parser.add_option(
        "--folder",
        action="store",
        default="C:\\work\\BlackMoon\\Closers.cocos\\client\\branches\\yougubt5\\Resources\\",
        dest="folder",
        help="游戏分支目录",
    )
    parser.add_option(
        "--root",
        action="store",
        dest="root",
        default="C:\\work\\BlackMoon\\Closers.cocos\\",
        help="要查询的文件目录",
    )

    parser.add_option(
        "--reset",
        action="store_true",
        dest="reset",
        # default=True,
        help="是否恢复GameVersion的修改(基于svn的版本)",
    )

    (opts, _) = parser.parse_args()

    if opts.folder == None:
        print("lost arg folder")
        sys.exit(-1)

    if opts.root == None:
        print("lost arg root")
        sys.exit(-2)

    pathSaveGameVersion = os.path.abspath(opts.folder + os.sep + PATH_GAME_RES_VERSION_LUA)
    pathRoot = os.path.abspath(opts.root)
    print("game_version_lua_path: " + pathSaveGameVersion)
    print("root_path: " + pathRoot)

    if opts.reset:
        # 重置版本号
        ResetVersion()
    else:
        WriteVersion()
