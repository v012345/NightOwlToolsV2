require "lua.Common"
require "lua.LuaFileDbSystem"
local from = arg[1]
local to = arg[2]

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
-- print(#lines)
local o = {}
local total = #lines
for i, v in ipairs(lines) do
    io.write(string.format("正在计算文件大小 : %s/%s\r", i, total))
    local from_file = from .. v
    local f = lfs.attributes(from_file)
    o[#o + 1] = {
        name = v,
        size = f.size >> 20
    }
end
table.sort(o, function(a, b)
    return a.size > b.size
end)

local file = io.open(to, "w") or error("can't open " .. to)
for i, v in ipairs(o) do
    file:write(v.name, ", ", v.size, "M\n")
end
file:close()
print()
