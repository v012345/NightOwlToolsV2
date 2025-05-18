require "lua.Common"

local excludeDir = { -- .git, .svn 之类, 不去比较
    [".git"] = true,
    [".svn"] = true,
    [".vscode"] = true
}
local from = arg[1]
local output_to = arg[2]
local x = {}
os.execute("chcp 65001 >nul")
Common.GetAllFilesOfDirectory(from, excludeDir, x)
local total = #x
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("获取 %s 下的文件: %s/%s\r",from,i,total))
    y[#y + 1] = string.gsub(v, from, "", 1)
end

local file = io.open(output_to, "w") or error("can't open " .. output_to)
file:write(table.concat(y, "\n"))
file:close()
print()
