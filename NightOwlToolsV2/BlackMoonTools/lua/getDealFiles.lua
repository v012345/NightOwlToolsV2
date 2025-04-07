require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local to = arg[2]
local output_to = arg[3]
lfs.mkdir(to)
local x = {}
Common.GetAllFilesOfDirectory(from, excludeDir, x)
Common.CopyDirStructure(from, to, excludeDir)
local total = #x
local y = {}
for i, v in ipairs(x) do
    print(i, total)
    Common.Copy(v, string.gsub(v, from, to, 1))
    y[#y + 1] = string.gsub(v, from, "", 1)
end

local file = io.open(output_to, "w") or error("can't open " .. output_to)
file:write(table.concat(y, "\n"))
file:close()
