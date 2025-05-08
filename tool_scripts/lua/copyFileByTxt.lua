require("lua.LuaPanda").start("127.0.0.1", 8818);
require "lua.Common"
local from = arg[1]
local to = arg[2]
local txt = arg[3]

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
local total = #lines
for i, v in ipairs(lines) do
    io.write(string.format("正在复制指定文件 : %s/%s\r", i, total))
    local from_file = from .. v
    local to_file = to .. v
    if lfs.attributes(from_file) then
        Common.Copy(from_file, to_file)
    else
        os.remove(to_file)
    end
end
print()
