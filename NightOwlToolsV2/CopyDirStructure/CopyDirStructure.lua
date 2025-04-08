require "CopyDirStructure.config"
local exclude = {}
for i, v in ipairs(CopyDirStructure.Exclude) do
    exclude[v] = i
end
CopyDirStructure.copyDirStructure(CopyDirStructure.Source, CopyDirStructure.Target, exclude)
