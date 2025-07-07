require "lua.Common"
local sha = require "extensions.pure_lua_SHA.sha2"
-- 希望支持 sqlite , 现在选使用文件, 快点出工具
LuaFileDB = {}
local DbType = "file"
local function Exist(bdName)
    if DbType == "file" then
        local append = sha.sha1(bdName) -- create calculation instance #1
        print(append)
    end
end

function LuaFileDB.Exist(bdName)
    Exist(bdName)
end

return LuaFileDB
