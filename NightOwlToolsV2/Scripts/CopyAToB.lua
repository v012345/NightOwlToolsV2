-- dirFrom 中的文件完成复制到 dirTo 中去

local dirFrom = "C:/work/Pokemon/20211020_3_3_0/03_Code/trunk/project/Resources"
local dirTo = "C:/work/Pokemon/20211020_3_3_0/03_Code/trunk/project/ResourcesStation"
lfs.mkdir(dirTo)
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

local function copyDirStructure(from, to, exclude)
    for entry in lfs.dir(from) do
        if entry ~= "." and entry ~= ".." then
            local path = from .. "/" .. entry
            local attri = lfs.attributes(path)
            if attri.mode == "directory" and not exclude[entry] then
                local target = to .. "/" .. entry
                if not lfs.attributes(target) then
                    lfs.mkdir(target)
                end
                copyDirStructure(path, target, exclude)
            end
        end
    end
end

local function copy(from, to)
    local windows_from = string.gsub(from, "/", "\\")
    local windows_to = string.gsub(to, "/", "\\")
    local cmd = string.format('copy "%s" "%s" /Y', windows_from, windows_to)
    local r = io.popen(cmd)
    if not r then
        error("can't run " .. cmd)
    else
        print(r:read("a"))
        r:close()
    end
end

local dirAFiles = {}
copyDirStructure(dirFrom, dirTo, excludeDir)
getAllFilesOfDirectory(dirFrom, excludeDir, dirAFiles)

for i, v in ipairs(dirAFiles) do
    print(v)
    -- print(string.gsub(v, dirFrom, dirTo, 1))
    copy(v, string.gsub(v, dirFrom, dirTo, 1))
end
