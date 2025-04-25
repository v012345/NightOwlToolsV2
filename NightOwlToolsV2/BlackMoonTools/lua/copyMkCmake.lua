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
print("正在获取")
print(from)
print("下的全部文件")
Common.GetAllFilesOfDirectory(from, excludeDir, x)
local total = #x
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("%s/%s\r", i, total))
    y[#y + 1] = string.gsub(v, from, "", 1)
end
print()
print("正在复制特定文件")
for i, v in ipairs(y) do
    io.write(string.format("%s/%s\r", i, total))
    local filename = string.lower(v)
    if string.match(filename, "android%.mk$") or string.match(filename, "application%.mk$") or
        string.match(filename, "cmakelists%.txt$") then
        Common.Copy(from .. v, to .. v)
    end
end
print()

