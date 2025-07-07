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
    io.write(string.format("正在过滤要求的文件 : %s/%s\r", i, total))
    for _, f in ipairs(config.files) do
        if string.match(v, f .. "$") then
            y[#y + 1] = v
        end
    end
end
local z = {}
total = #y
print()
for i, v in ipairs(y) do
    print(v)
    io.write(string.format("正在计算文件 md5 : %s/%s\r", i, total))
    local md5 = Common.Checksum(v)
    if not z[md5] then
        z[md5] = {}
    end
    z[md5][#z[md5] + 1] = v
end
print()
for k, v in pairs(z) do
    print(k)
    for i, f in ipairs(v) do
        print(i, f)
    end
end
print()
