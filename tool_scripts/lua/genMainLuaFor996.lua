require "lua.Common"
local txt = arg[1]
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
local total = #lines
local y = {}
for i, v in ipairs(lines) do
    io.write(string.format("正在生成 require 指令 : %s/%s\r", i, total))
    v = string.gsub(v, "%.lua$", "", 1)
    v = string.gsub(v, "/", ".")
    y[#y + 1] = string.format('require "no_run%s"', v)
end
local file = io.open(to, "w") or error("can't open " .. to)
file:write(table.concat(y, "\n"))
file:close()
print("")
