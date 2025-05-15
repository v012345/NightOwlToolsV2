require "lua.Common"
local from = arg[1]
local output_to = arg[2]
local configStr = arg[3]
local func, err = load(configStr, "configStr")
if not func then
    print("Error loading config: ", err)
    return
end
local config = func()
local excludeDir = config.exclude
local ext = config.ext

local x = {}
os.execute("chcp 65001 >nul")
print(string.format("正在获取 %s 下的全部文件",from))
Common.GetAllFilesOfDirectory(from, excludeDir, x)
local total = #x
local y = {}
for i, v in ipairs(x) do
    io.write(string.format("正在过滤文件 %s/%s\r", i, total))
    if ext[v:match("^.+(%..+)$")] then
        y[#y + 1] = string.gsub(v, from, "", 1)
    end
end
print()

local file = io.open(output_to, "w") or error("can't open " .. output_to)
file:write(table.concat(y, "\n"))
file:close()
print(string.format("已经输出到 %s", output_to))
