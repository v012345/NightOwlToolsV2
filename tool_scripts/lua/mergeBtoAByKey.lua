require "lua.Common"

local A = require(arg[1])
local B = require(arg[2])
local to = arg[3]

local function mergeBToABykey(A, B)
    for k, v in pairs(B) do
        if not A[k] then
            A[k] = v
        else
            if type(v) == "table" then
                mergeBToABykey(A[k], v)
            end
        end
    end
end

mergeBToABykey(A, B)

local file = io.open(to, "w") or error("can't open " .. to)

file:write("local config = {\n")
local function dump(t, f, i)
    for k, v in pairs(t) do
        if type(v) == "table" then
            if type(k) == "number" then
                f:write(string.rep("\t", i), string.format("[%s] = {\n", k))
            else
                if i == 1 then -- 第一层的键是 ["key"] = { 这样的
                    f:write(string.rep("\t", i), string.format("[\"%s\"] = {\n", k))
                else
                    f:write(string.rep("\t", i), string.format("%s = {\n", k))
                end
            end
            dump(v, f, i + 1)
            f:write(string.rep("\t", i), "},\n")
        else
            local value = ""
            if type(v) == "number" then
                value = v
            else
                value = string.gsub(value, "\\", "\\\\")
                value = string.gsub(value, "\"", "\\\"")
                value = '\"' .. v .. '\"'
            end
            value = string.gsub(value, "\n", "\\n")
            if type(k) == "number" then
                f:write(string.rep("\t", i), string.format("[%s] =%s,\n", k, value))
            else
                f:write(string.rep("\t", i), string.format("%s =%s,\n", k, value))
            end
        end

    end
end
dump(A, file, 1)
file:write("}\n")
file:write("return config\n")
file:close()
