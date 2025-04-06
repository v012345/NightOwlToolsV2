-- 因为要对文件进行处理, 非常浪费时间, 所以会缓存文件
require "Tools.Win32Cmd"
require "Tools.Common"
-- 游戏资源原路径
local origin = "C:/work/BlackMoon/Closers.cocos/client/branches/yougubt5/Resources"
local dirTo = "C:/work/NightOwlToolsV2/NightOwlToolsV2/temp/yougubt5/Resources"
local res_db = "C:/work/NightOwlToolsV2/NightOwlToolsV2/temp/yougubt5/res.db"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true,
    ["logDir"] = true
}

do -- 生成目标文件目录结构
    if not lfs.attributes(dirTo) then
        Win32.makeDirectory(dirTo)
    end
    Common.copyDirStructure(origin, dirTo, excludeDir)
end


local db = nil
do -- 看看有没有存文件状态的 db 文件
    db = sqlite3.open(res_db)
    local query = "SELECT name FROM sqlite_master WHERE type='table' AND name='%s'"
    local iterator, tables = db:nrows(string.format(query, "file_states"))
    -- if not exist, create it
    if not iterator(tables) then
        query = "CREATE TABLE %s (path TEXT PRIMARY KEY, modification TIMESTAMP,checksum TEXT);"
        db:exec(string.format(query, "file_states"))
    end
    db:close()

    db = sqlite3.open_memory()
    local query = "ATTACH DATABASE '%s' AS disk_db"
    assert(db:exec(string.format(query, res_db)) == sqlite3.OK)
    query = "CREATE TABLE %s AS SELECT * FROM disk_db.%s"
    assert(db:exec(string.format(query, "file_states", "file_states")) == sqlite3.OK)
    assert(db:exec("DETACH DATABASE disk_db") == sqlite3.OK)
end

local origin_all_file = {}
Common.getAllFilesOfDirectory(origin, excludeDir, origin_all_file)
print(#origin_all_file)

local created, modified, touched, unchanged = {}, {}, {}, {}
local total = #origin_all_file
local query = "SELECT * FROM %s WHERE path = ?"
local stmt = db:prepare(string.format(query, "file_states"))
for i, path in ipairs(origin_all_file) do
    stmt:bind_values(path)
    local result = stmt:step()
    local db_file_state = nil
    if result == sqlite3.ROW then
        db_file_state = stmt:get_named_values()
    end
    stmt:reset()
    if db_file_state then
        local modification = lfs.attributes(path, "modification")
        if not modification then
            print(path)
        end
        if db_file_state.modification < modification then
            local checksum = Common.Checksum(path)
            if checksum ~= db_file_state.checksum then
                table.insert(modified, path)
            else
                table.insert(touched, path)
            end
        else
            table.insert(unchanged, path)
        end
    else
        table.insert(created, path)
    end
    Common.Write(string.format("check  files : %s/%s", i, total))
end
if total > 0 then
    print()
end
stmt:finalize()

