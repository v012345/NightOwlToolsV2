local plistCsi = "D:/Closers.cocos/resource/ui/branches/dzogame_sea_v1/zhcn/cocosstudio/plist"
local imageRes = "D:/Closers.cocos/resource/ui/branches/dzogame_sea_v1/zhcn/cocosstudio/image"
local uiCsd = "D:/Closers.cocos/resource/ui/branches/dzogame_sea_v1/zhcn/cocosstudio/ui"

local imageFiles = PublishResource.CollectImageRess(imageRes)
local uiFiles = PublishResource.CollectFiles(uiCsd, ".csd")
local plistFiles = PublishResource.CollectFiles(plistCsi, ".csi")
print(#plistFiles)

os.remove('LocalOnly/BlackMoonUi.db')
local db = sqlite3.open('LocalOnly/BlackMoonUi.db')
print(db:exec [[
    CREATE TABLE dzogame_sea_v1 (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        modification TIMESTAMP,
        sha1 TEXT,
        name TEXT,
        relative_path TEXT
    );
    CREATE INDEX idx_relative_path ON dzogame_sea_v1(relative_path);
]])

local stmt = db:prepare [[ INSERT INTO 
    dzogame_sea_v1 (modification,sha1,name,relative_path)
    VALUES (:modification,:sha1,:name,:relative_path)
]]
print("Failed to prepare statement: " .. db:errmsg())
print(stmt)
local lennn = #plistCsi
local all = #plistFiles
for i, v in ipairs(plistFiles) do
    Common.ShowOnOneline(string.format("%s/%s %s", i, all, v.name))
    stmt:bind_names({
        modification = v.modification,
        sha1 = Win32.getSha1(v.path),
        name = v.name,
        relative_path = string.sub(v.path, lennn, #v.path)
    })
    stmt:step()
    stmt:reset()
end
stmt:finalize()

for row in db:nrows("SELECT * FROM dzogame_sea_v1") do
    print(row.id, row.name)
end
