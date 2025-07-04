require "lua.Common"
local from = arg[1]
local to = arg[2]
local configStr = arg[3]
local x = {}
os.execute("chcp 65001 >nul")
if not lfs.attributes(to) then
    lfs.mkdir(to)
end
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
Common.GetAllFilesOfDirectory(from, excludeDir, x)

local total = #x
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("正在获取文件的相对路径 : %s/%s\r", i, total))
    y[#y + 1] = string.gsub(v, from, "", 1)
end
print()
for i, v in ipairs(y) do
    io.write(string.format("正在复制文件到 %s : %s/%s\r", to, i, total))
    Common.Copy(from .. v, to .. v)
end
print()
