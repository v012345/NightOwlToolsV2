-- 比较两个文件夹的目录结构
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

for k, v in pairs(dirAMap) do
    if dirBMap[k] then
        dirBMap[k] = nil
        dirAMap[k] = nil
    end
end
print(">>>>>> DirA <<<<<<")
for k, v in pairs(dirAMap) do
    print(k)
end
print(">>>>>> DirB <<<<<<")
for k, v in pairs(dirBMap) do
    print(k)
end
