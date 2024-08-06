for i, project in ipairs(PublishResource.Projects) do
    local cocosstudio_directory = project.source .. "/cocosstudio"
    local ui_directory = cocosstudio_directory .. "/ui"
    local file_path = PublishResource.GetFilesOfDir(ui_directory, "csd")
    local to_publish_path, _, to_update_modification = PublishResource.CheckModification(file_path)
    for _, path in ipairs(to_update_modification) do
        PublishResource.UpdateOrInsert({
            path = path,
            modification = lfs.attributes(path, "modification"),
        })
    end

    local to_publish_file = {}
    local root_path = ui_directory .. "/"
    for i, path in ipairs(to_publish_path) do
        to_publish_file[i] = string.gsub(path, root_path, "", 1)
    end
    PublishResource.PublishUi(to_publish_file, project.source, project.target)
    for i, path in ipairs(to_publish_path) do
        PublishResource.UpdateOrInsert({
            path = path,
            modification = lfs.attributes(path, "modification"),
            sha1 = Win32.getSha1(path)
        })
        Common.ShowOnOneline(string.format("update db %s/%s", i, #to_publish_path))
    end
    if #to_publish_path > 0 then
        print()
    end
    print("publish " .. #to_publish_path .. " files")
end
