require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local to = arg[2]
local x = {}
os.execute("chcp 65001 >nul")
if not lfs.attributes(to) then
    lfs.mkdir(to)
end
Common.GetAllFilesOfDirectory(from, excludeDir, x)
local total = #x
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("正在获取 %s 下的文件 : %s/%s\r", from, i, total))
    y[#y + 1] = string.gsub(v, from, "", 1)
end
print()
for i, v in ipairs(y) do
    io.write(string.format("正在复制文件到 %s : %s/%s\r", to, i, total))
    Common.Copy(from .. v, to .. v)
end
print()
