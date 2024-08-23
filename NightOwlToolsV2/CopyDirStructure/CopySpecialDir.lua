local function CopySpecialDir(from, to)
    for entry in lfs.dir(from) do
        if entry ~= "." and entry ~= ".." then
            local path = from .. "/" .. entry
            -- print(path)
            local attri = lfs.attributes(path)
            if attri and attri.mode == "directory" then
                local target = to .. "/" .. entry
                if string.lower(entry) == "language" and not lfs.attributes(target) then
                    local win_type_target = string.gsub(target, "/", "\\")
                    local cmd = io.popen(string.format('mkdir "%s"', win_type_target), "r") or error()
                    local _ = cmd:read("a")
                    cmd:close()
                    CopyDirStructure.copyDirStructure(from .. "/" .. entry, target, {})
                    print(target)
                else
                    CopySpecialDir(path, target)
                end
            end
        end
    end
end
local from = "D:\\20240731_1_2_0_launch\\02_Resources\\FGUI\\SengokuFGUI\\assets"
local to = "D:\\20240731_1_2_0_launch\\02_Resources\\FGUI\\SengokuFGUI\\assets_VI"
CopySpecialDir(from, to)
