require "lua.Common"
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

local total = #lines
for i, v in ipairs(lines) do
    io.write(string.format("正在删除文件 : %s/%s\r", i, total))
    local from_file = from .. v
    os.remove(from_file)
end
print()
