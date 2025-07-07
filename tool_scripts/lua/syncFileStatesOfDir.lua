require "lua.Common"
require "lua.LuaFileDbSystem"

local from = arg[1]
local txt = arg[2]

local lines = {}
local file = io.open(txt, "r") -- 以只读模式打开文件
if file then
    for line in file:lines() do
        table.insert(lines, line)
    end
    file:close()
else
    print("无法打开文件")
end

LuaFileDB.CreateIfNotExist(from)
local db = LuaFileDB.Open(from)
local total = #lines
local y = {}
for i, v in ipairs(lines) do
    io.write(string.format("正在同步文件状态 %s/%s\r", i, total))
    -- local key = string.gsub(v, from, "", 1)
    db[v] = {
        modified_time = lfs.attributes(v, "modification"),
        md5 = Common.Checksum(from .. v)
    }
end
print()
db:Save()
print(string.format("已经写入 [\"%s\"] 数据库中", from))

