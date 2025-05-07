require "lua.Common"
local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true,
    [".cxx"] = true,
    ["build"] = true,
}
os.execute("chcp 65001 >nul")
local root = arg[1]
local to = arg[2]
local lines = {}
local file = io.open(arg[3], "r") -- 以只读模式打开文件
if file then
    for line in file:lines() do
        table.insert(lines, line)
    end
    file:close()
else
    print("无法打开文件")
end

-- local total = #lines
-- for i, v in ipairs(lines) do
--     io.write(string.format("%s/%s\r", i, total))
--     if string.match(v, "^" .. root, 1) then
--         local f = string.gsub(v, root, "", 1)
--         Common.Copy(v, to .. f)
--     end
-- end
print()

local x = {}

print("正在获取")
print(root)
print("下的全部文件")
Common.GetAllFilesOfDirectory(root, excludeDir, x)
total = #x
print("正在复制特定文件")
for i, v in ipairs(x) do
    io.write(string.format("%s/%s\r", i, total))
    local filename = string.lower(v)
    if string.match(filename, "%.a$") or string.match(filename, "%.so$") or
        string.match(filename, "cmakelists%.txt$") then
        Common.Copy(v, string.gsub(v, root, to, 1))
    end
end
print()
-- print(#lines)
