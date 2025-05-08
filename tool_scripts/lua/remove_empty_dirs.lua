-- 递归删除空文件夹
local function remove_empty_dirs(path)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local fullpath = path .. "/" .. file
            local attr = lfs.attributes(fullpath)
            if attr and attr.mode == "directory" then
                remove_empty_dirs(fullpath)
            end
        end
    end

    -- 再次检查目录是否空
    local is_empty = true
    for _ in lfs.dir(path) do
        if _ ~= "." and _ ~= ".." then
            is_empty = false
            break
        end
    end

    if is_empty then
        os.remove(path) -- 不能删目录？
        lfs.rmdir(path) -- 用这个
    end
end
print("正在清理空目录")
remove_empty_dirs(arg[1])
