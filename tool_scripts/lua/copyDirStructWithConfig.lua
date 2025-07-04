require "lua.Common"
local from = arg[1]
local to = arg[2]
local configStr = arg[3]
local func, err = load(configStr, "configStr")
if not func then
    print("Error loading config: ", err)
    return
end
local config = func()
local excludeDir = {}
for k, v in pairs(config.exclude) do
    excludeDir[v] = true
end
local ext = config.ext
lfs.mkdir(to)
os.execute("chcp 65001 >nul")
print(string.format("正在复制 %s 目录结构到 %s 中", from, to))
print(configStr)
Common.CopyDirStructure(from, to, excludeDir)
