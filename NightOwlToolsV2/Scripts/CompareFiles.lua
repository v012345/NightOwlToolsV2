-- 比较两个文件夹的目录结构
local dirB = "C:\\work\\Pokemon\\Pokemon.android_studio.x7\\app\\src\\main\\assets" -- 使用/
local dirA = "C:\\work\\Pokemon\\20211020_3_3_0\\03_Code\\trunk\\project\\ResourcesStation.encrypt" -- 使用/

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}

local function getAllFilesOfDirectory(directory, exclude, result)
    for entry in lfs.dir(directory) do
        if entry ~= "." and entry ~= ".." then
            local path = directory .. "/" .. entry
            local attri = lfs.attributes(path)
            if attri.mode == "file" then
                result[#result + 1] = path
            else
                if not exclude[entry] then
                    getAllFilesOfDirectory(path, exclude, result)
                end
            end
        end
    end
end
local dirAFiles = {}
local dirBFiles = {}
getAllFilesOfDirectory(dirA, excludeDir, dirAFiles)
local diff = {}
local miss = {}
local function files_are_equal_quick(f1, f2)
    local x1 = io.open(f1, "rb")

    if not x1 then
        return false
    end
    local x2 = io.open(f2, "rb")
    if not x2 then
        x1:close()
        return false
    end
    local data1 = x1:read("*a")
    local data2 = x2:read("*a")
    x1:close()
    x2:close()
    return data1 == data2
end
local total = #dirAFiles
for i, v in ipairs(dirAFiles) do
    print(i, total)
    local f1 = v
    local f2 = string.gsub(v, dirA, dirB, 1)
    print(f2)
    if lfs.attributes(f2) then
        if not files_are_equal_quick(f1, f2) then
            diff[#diff + 1] = v
        end
    else
        miss[#miss + 1] = v
    end
end
local f1 = io.open("miss.txt", "w") or error("j")
local f2 = io.open("diff.txt", "w") or error("j")
f1:write(table.concat(miss, "\n"))
f2:write(table.concat(diff, "\n"))
f1:close()
f2:close()

-- getAllFilesOfDirectory(dirB, excludeDir, dirBFiles)
-- local function createFilesMap(root, files)
--     local res = {}
--     local i = #root + 1
--     for _, v in ipairs(files) do
--         res[string.sub(v, i)] = true
--     end
--     return res
-- end
-- local dirAMap = createFilesMap(dirA, dirAFiles)
-- local dirBMap = createFilesMap(dirB, dirBFiles)

-- for k, v in pairs(dirAMap) do
--     if dirBMap[k] then
--         dirBMap[k] = nil
--         dirAMap[k] = nil
--     end
-- end
-- print(">>>>>> DirA <<<<<<")
-- for k, v in pairs(dirAMap) do
--     print(k)
-- end
-- print(">>>>>> DirB <<<<<<")
-- for k, v in pairs(dirBMap) do
--     print(k)
-- end
