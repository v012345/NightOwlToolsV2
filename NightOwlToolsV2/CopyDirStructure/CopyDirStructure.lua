require "CopyDirStructure.config"
local exclude = {}
for i, v in ipairs(CopyDirStructure.Exclude) do
    exclude[v] = i
end
local function copyDirStructure(from, to)
    for entry in lfs.dir(from) do
        if entry ~= "." and entry ~= ".." then
            local path = from .. "/" .. entry
            local attri = lfs.attributes(path)
            if attri.mode == "directory" and not exclude[entry] then
                local target = to .. "/" .. entry
                if not lfs.attributes(target) then
                    lfs.mkdir(target)
                    print(target)
                end
                copyDirStructure(path, target)
            end
        end
    end
end
copyDirStructure(CopyDirStructure.Source, CopyDirStructure.Target)
