require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local output_dir = arg[2]
local output_to = arg[3]
local x = {}
os.execute("chcp 65001 >nul")
print("正在获取")
print(from)
print("中的资源")

local function initDB(DBPath, TableName)
    local db = sqlite3.open(DBPath)
    local query = "SELECT name FROM sqlite_master WHERE type='table' AND name='%s'"
    local iterator, tables = db:nrows(string.format(query, TableName))
    -- if not exist, create it
    if not iterator(tables) then
        query = "CREATE TABLE %s (path TEXT PRIMARY KEY, modification TIMESTAMP,checksum TEXT);"
        db:exec(string.format(query, TableName))
    end
    db:close()
end

local function openDB(DBPath, TableName)
    local db = sqlite3.open_memory()
    local query = "ATTACH DATABASE '%s' AS disk_db"
    assert(db:exec(string.format(query, DBPath)) == sqlite3.OK)
    query = "CREATE TABLE %s AS SELECT * FROM disk_db.%s"
    assert(db:exec(string.format(query, TableName, TableName)) == sqlite3.OK)
    assert(db:exec("DETACH DATABASE disk_db") == sqlite3.OK)
    return db
end


local function getStateByPath(db, path)
    local query = "SELECT * FROM %s WHERE path = ?"
    local stmt = db:prepare(string.format(query, "file_states"))
    stmt:bind_values(path)
    local result = stmt:step()
    local row = nil
    if result == sqlite3.ROW then
        row = stmt:get_named_values()
    end
    stmt:reset()
    stmt:finalize()
    return row
end

local DBPath = output_dir .. "/OriginFileStatus.db"
local TableName = "file_states"
initDB(DBPath, TableName)
local db = openDB(DBPath, TableName)

local function GetAllFilesOfDirectory(directory, exclude, result, i)
    for entry in lfs.dir(directory) do
        if entry ~= "." and entry ~= ".." then
            local path = directory .. "/" .. entry
            local attri = lfs.attributes(path)
            if attri.mode == "file" then
                result[#result + 1] = path
                io.write(string.format("已收集到 %s 个文件\r", i[1]))
                i[1] = i[1] + 1
            else
                if not exclude[entry] then
                    GetAllFilesOfDirectory(path, exclude, result, i)
                end
            end
        end
    end
end

GetAllFilesOfDirectory(from, excludeDir, x, {1})
print()
print("检查文件是否变动(与上次出资源时对比)")

local total = #x
local need_deal = 0
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("%s/%s\r", i, total))
    local path = string.gsub(v, from, "", 1)
    local db_file_state = getStateByPath(db, path)
    if db_file_state then
        local modification = lfs.attributes(v, "modification")
        if db_file_state.modification ~= modification then
            local checksum = Common.Checksum(v)
            if checksum ~= db_file_state.checksum then
                y[#y + 1] = path
                need_deal = need_deal + 1
            else
                local query = string.format("UPDATE %s SET modification = %s WHERE path = '%s';", TableName,
                    modification, path)
                db:exec(query)
            end
        end
    else
        y[#y + 1] = path
        need_deal = need_deal + 1
    end
end

local file = io.open(output_to, "w") or error("can't open " .. output_to)
file:write(table.concat(y, "\n"))
file:close()

db:close()