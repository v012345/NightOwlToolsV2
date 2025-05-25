-- 注意, 不想写的太通用, 就是简单比较
os.execute("chcp 65001 >nul")
require "lua.Common"

local t1 = require(arg[1])
local t2 = require(arg[2])
local output_to = arg[3]
local file = io.open(output_to, "w") or error("can't open " .. output_to)
file:write()

local del = true
local function isSameKeyWithSameValue(A, B)
    for k, v in pairs(B) do
        if A[k] then
            if type(v) == "table" and type(A[k]) == "table" then
                isSameKeyWithSameValue(A[k], v)
            else
                if A[k] ~= v then
                    file:write(arg[1], "   diff   ", k, "  ", A[k], " ", v, "\n")
                    del = false
                end
            end
        end
    end
end

isSameKeyWithSameValue(t1, t2)

file:close()
if del then
    os.remove(output_to)
end
