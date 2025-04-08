-- 比较两个文件夹内文件
local dirA = "C:/Users/Meteor/Desktop/temp/publishRes" -- 使用/
local dirB = "C:/work/NightOwlToolsV2/NightOwlToolsV2/BlackMoonTools/temp/bl/yougubt5/android/res_compressed" -- 使用/

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
getAllFilesOfDirectory(dirB, excludeDir, dirBFiles)
local function createFilesMap(root, files)
    local res = {}
    local i = #root + 1
    for _, v in ipairs(files) do
        res[string.sub(v, i)] = true
    end
    return res
end
local dirAMap = createFilesMap(dirA, dirAFiles)
local dirBMap = createFilesMap(dirB, dirBFiles)
local dirAMap_copy = {}
for k, v in pairs(dirAMap) do
    dirAMap_copy[#dirAMap_copy+1] = k
    if dirBMap[k] then
        dirBMap[k] = nil
        dirAMap[k] = nil
    end
end
print(">>>>>> DirA <<<<<<")
local dont_care = {}
for k, v in pairs(dirAMap) do
    print(k)
    dont_care[k] = true
end
print(">>>>>> DirB <<<<<<")
for k, v in pairs(dirBMap) do
    print(k)
    dont_care[k] = true
end

local diff = {}
local miss = {}
local function files_are_equal_quick(f1, f2)
    local x1 = io.open(f1, "rb")
    local x2 = io.open(f2, "rb")
    local data1 = x1:read("*a")
    local data2 = x2:read("*a")
    x1:close()
    x2:close()
    return data1 == data2
end
local total = #dirAMap_copy
print(total)
for i, v in ipairs(dirAMap_copy) do
    io.write(string.format("%s/%s\r", i, total))
    if not dont_care[v] then
        if not files_are_equal_quick(dirA .. v, dirB .. v) then
            diff[#diff + 1] = v
        end
    end

end

local f2 = io.open("diff.txt", "w") or error("j")
f2:write(table.concat(diff, "\n"))
f2:close()
