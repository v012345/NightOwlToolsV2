require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local output_dir = arg[2]
local file_state_path = arg[3]
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

local function realseDB(db, DBPath, tableName)
    local query = "ATTACH DATABASE '%s' AS disk_db"
    assert(db:exec(string.format(query, DBPath)) == sqlite3.OK)
    query = "INSERT OR REPLACE INTO disk_db.%s SELECT * FROM %s"
    assert(db:exec(string.format(query, tableName, tableName)) == sqlite3.OK)
    assert(db:exec("DETACH DATABASE disk_db") == sqlite3.OK)
    db:close()
end



local function countTableRows(db, tableName)
    local query = string.format("SELECT COUNT(*) AS count FROM %s", tableName)
    local stmt = db:prepare(query)
    local result = stmt:step()
    local count = 0
    if result == sqlite3.ROW then
        local row = stmt:get_named_values()
        count = row.count
    end
    stmt:finalize()
    return count
end


local DBPath = output_dir .. "/OriginFileStatus.db"
local TableName = "file_states"
initDB(DBPath, TableName)
local db = openDB(DBPath, TableName)
print()
print(countTableRows(db, TableName))
print("更新文件状态数据库")

local need_update_files = {}

for line in io.lines(file_state_path) do
    line = line:match("^%s*(.-)%s*$") -- 去掉前后空白
    if line ~= "" then
        table.insert(need_update_files, line)
    end
end

local total = #need_update_files
for i, v in ipairs(need_update_files) do
    io.write(string.format("%s/%s\r", i, total))
    local modification = lfs.attributes(from .. v, "modification")
    local checksum = Common.Checksum(from .. v)
    local query = string.format("INSERT OR REPLACE INTO %s (path, modification, checksum) VALUES ('%s', %s, '%s');",
        TableName, v, modification, checksum)
    db:exec(query)
end

realseDB(db, DBPath, TableName)

