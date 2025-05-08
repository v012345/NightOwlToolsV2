require "lua.Common"
local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true,
    [".cxx"] = true,
    ["build"] = true
}
os.execute("chcp 65001 >nul")
local root = arg[1]
local to = arg[2]
local x = {}

print(string.format("正在获取 %s 下的全部文件", root))
Common.GetAllFilesOfDirectory(root, excludeDir, x)
local total = #x
for i, v in ipairs(x) do
    io.write(string.format("正在复制 .a .so CMakeLitst 文件 : %s/%s\r", i, total))
    local filename = string.lower(v)
    if string.match(filename, "%.a$") or string.match(filename, "%.so$") or string.match(filename, "cmakelists%.txt$") then
        Common.Copy(v, string.gsub(v, root, to, 1))
    end
end
print()
