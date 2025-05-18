require "lua.Common"
local tableHead = require(arg[1]).py
local output = arg[2]

local ik = {}
for k, v in pairs(tableHead.ki) do
    ik[v] = k
end

local file = io.open(output, "w")
file:write(string.format("%s = {\n", arg[3]))
file:write(string.format('    "key": "%s",\n', tableHead.key))
local key_index = -1
for k, v in pairs(tableHead.ki) do
    if tableHead.key == k then
        key_index = v - 1
        break
    end
end
file:write(string.format('    "key_index": %s,\n', key_index))
file:write('    "head": [\n')

for i, v in ipairs(ik) do
    file:write(string.format('        "%s",\n', v))
end
file:write("    ],\n")
file:write("}\n")
file:close()
print(string.format("生成 %s 的表头 py 文件",  arg[3]))
