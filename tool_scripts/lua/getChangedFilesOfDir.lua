require "lua.Common"
require "lua.LuaFileDbSystem"

local from = arg[1]
local output_to = arg[2]
local configStr = arg[3]
local func, err = load(configStr, "configStr")
if not func then
    print("Error loading config: ", err)
    return
end
local config = func()
local excludeDir = {}
for k, v in ipairs(config.exclude) do
    excludeDir[v] = true
end
local x = {}
print(string.format("正在获取 %s 下的全部文件", from))
Common.GetAllFilesOfDirectory(from, excludeDir, x)
LuaFileDB.CreateIfNotExist(from)
local db = LuaFileDB.Open(from)
local total = #x
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("正在检查文件状态 %s/%s\r", i, total))
    local key = string.gsub(v, from, "", 1)
    local fileInfo = db[key]
    if not fileInfo then
        y[#y + 1] = key
    else
        if fileInfo["modified_time"] ~= lfs.attributes(v, "modification") then
            if Common.Checksum(v) ~= fileInfo["md5"] then
                y[#y + 1] = key
            end
        end
    end

end
print()
print("需处理文件数", #y)
local file = io.open(output_to, "w") or error("can't open " .. output_to)
file:write(table.concat(y, "\n"))
file:close()
print(string.format("已经输出到 %s", output_to))

