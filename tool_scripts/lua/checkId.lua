require "lua.Common"

local A = require(arg[1])

local maybe_id = {}

for k, t in pairs(A) do
    if type(t) == "table" then
        for k, v in pairs(t) do
            maybe_id[k] = true
        end
    else
        print("奇怪的表 " .. A)
        return
    end
end

for key, t in pairs(A) do
    for k, v in pairs(t) do
        if key ~= v then
            maybe_id[k] = false
        end
    end
end

for k, v in pairs(maybe_id) do
    if v then
        print(arg[1], k)
    end
end
