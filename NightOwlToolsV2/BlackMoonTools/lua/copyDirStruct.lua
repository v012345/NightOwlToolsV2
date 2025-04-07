require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local to = arg[2]
lfs.mkdir(to)
Common.CopyDirStructure(from, to, excludeDir)