require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local to = arg[2]
lfs.mkdir(to)
os.execute("chcp 65001 >nul")
print("正在复制目录结构")
print(from)
print("复制到")
print(to)
Common.CopyDirStructure(from, to, excludeDir)