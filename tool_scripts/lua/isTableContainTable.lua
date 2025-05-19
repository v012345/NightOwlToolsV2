-- 注意, 不想写的太通用, 就是简单比较
os.execute("chcp 65001 >nul")
require "lua.Common"

local t1 = require(arg[1])
local t2 = require(arg[2])
local function is_A_contain_B(A, B)
    for k, v in pairs(B) do
        if A[k] then
            for i, j in pairs(v) do
                if A[k][i] ~= j then
                    if not (j == "" and A[k][i] == nil) then
                        if tostring(j) ~= tostring(A[k][i]) then
                            -- print(k, i, A[k][i], j)
                            return false
                        end
                    end
                end
            end
        else
            -- print(k, v)
            return false
        end
    end
    return true
end
if not is_A_contain_B(t1, t2) then
    print(arg[1], "不包含", arg[2])
end
