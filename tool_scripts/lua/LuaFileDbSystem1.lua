require "lua.Common"
local sha = require "extensions.pure_lua_SHA.sha2"
-- 希望支持 sqlite , 现在选使用文件, 快点出工具
LuaFileDB = {}
local DbType = "file"
local function Exist(bdName)
    if DbType == "file" then
        if not lfs.attributes(string.format("temp/%s.lua", tostring(sha.sha1(bdName)))) then
            return false
        else
            return true
        end
    end
end

local function Create(bdName, force)
    if DbType == "file" then
        if force then
            io.open(string.format("temp/%s.lua", tostring(sha.sha1(bdName))), "w"):write("return {}"):close()
        else
            if not LuaFileDB.Exist(bdName) then
                io.open(string.format("temp/%s.lua", tostring(sha.sha1(bdName))), "w"):write("return {}"):close()
            end
        end
    end
end

local function Open(bdName)
    if DbType == "file" then
        local dbFile = string.format("temp.%s", tostring(sha.sha1(bdName)))
        local LuaTable = require(dbFile)
        local mt = {
            __index = function(table, key)
                return LuaTable[key]
            end,
            __newindex = function(table, key, value)
                LuaTable[key] = value
            end,
            __pairs = function(tbl)
                return pairs(LuaTable)

            end,
            __ipairs = function(tbl)
                return ipairs(LuaTable)
            end
        }
        local db = {
            Save = function(self)
                local function save(tbl, file, compressed)
                    local f, err = io.open(file, "w")
                    if err then
                        print(err)
                        return
                    end
                    local indent = 1

                    -- local functions to make things easier
                    local function exportstring(s)
                        s = string.format("%q", s)
                        s = s:gsub("\\\n", "\\n")
                        s = s:gsub("\r", "")
                        s = s:gsub(string.char(26), "\"..string.char(26)..\"")
                        return s
                    end
                    local function serialize(o)
                        if type(o) == "number" then
                            f:write(o)
                        elseif type(o) == "boolean" then
                            if o then
                                f:write("true")
                            else
                                f:write("false")
                            end
                        elseif type(o) == "string" then
                            f:write(exportstring(o))
                        elseif type(o) == "table" then
                            f:write("{" .. (compressed and "" or "\n"))
                            indent = indent + 1
                            local tab = ""
                            for i = 1, indent do
                                tab = tab .. "    "
                            end
                            for k, v in pairs(o) do
                                f:write((compressed and "" or tab) .. "[")
                                serialize(k)
                                f:write("]" .. (compressed and "=" or " = "))
                                serialize(v)
                                f:write("," .. (compressed and "" or "\n"))
                            end
                            indent = indent - 1
                            tab = ""
                            for i = 1, indent do
                                tab = tab .. "    "
                            end
                            f:write((compressed and "" or tab) .. "}")
                        else
                            print("unable to serialzie data: " .. tostring(o))
                            f:write("nil," ..
                                        (compressed and "" or " -- ***ERROR: unsupported data type: " .. type(o) ..
                                            "!***"))
                        end
                    end

                    f:write("return {" .. (compressed and "" or "\n"))
                    local tab = "    "
                    for k, v in pairs(tbl) do
                        f:write((compressed and "" or tab) .. "[")
                        serialize(k)
                        f:write("]" .. (compressed and "=" or " = "))
                        serialize(v)
                        f:write("," .. (compressed and "" or "\n"))
                    end
                    f:write("}")
                    f:close()
                end
                save(LuaTable, string.format("temp/%s.lua", tostring(sha.sha1(bdName))))
            end
        }
        setmetatable(db, mt)
        return db
    end
end

function LuaFileDB.Exist(bdName)
    return Exist(bdName)
end

--- func desc
---@param bdName any
---@param force boolean 
function LuaFileDB.Create(bdName, force)
    Create(bdName, force)
end

function LuaFileDB.Open(bdName)
    if not LuaFileDB.Exist(bdName) then
        return false, "Not Exist"
    else
        return Open(bdName)
    end
end

return LuaFileDB
