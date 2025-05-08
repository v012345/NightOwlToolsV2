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
print(string.format("正在复制 %s 目录结构到 %s 中", from, to))
Common.CopyDirStructure(from, to, excludeDir)
