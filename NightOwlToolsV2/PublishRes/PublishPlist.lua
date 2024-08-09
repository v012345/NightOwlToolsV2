for i, project in ipairs(PublishRes.Projects) do
    local cocosstudio_directory = project.source .. "/cocosstudio"
    local plist_directory = cocosstudio_directory .. "/plist"
    local plist_files = PublishRes.GetFilesOfDir(plist_directory, "csi")
    local new_file, modified, to_update, unchanged = PublishRes.CheckFileState(plist_files)

    local must_publish = {table.unpack(new_file)}
    table.move(modified, 1, #modified, #must_publish + 1, must_publish)

    local may_publish = {table.unpack(unchanged)}
    table.move(to_update, 1, #to_update, #may_publish + 1, may_publish)

    PublishRes.UpdateModification(to_update)

    for i, path in ipairs(may_publish) do
        Common.ShowOnOneline(string.format("double check : %s/%s", i, #may_publish))
        local image_nodes = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
        local image_files = {}
        for i, node in ipairs(image_nodes) do
            image_files[i] = project.source .. "/cocosstudio/" .. node:getAttributeValue("Path")
        end
        local new_file, modified, to_update = PublishRes.CheckImageState(image_files)
        if next(new_file) or next(modified) then
            must_publish[#must_publish + 1] = path
        else
            PublishRes.UpdateModification(to_update)
        end
    end
    if #may_publish > 0 then
        print()
    end
    if #must_publish > 0 then
        PublishRes.PublishPlist(must_publish, project.source, project.target)
    end

    for i, path in ipairs(must_publish) do
        Common.ShowOnOneline(string.format("update plist : %s/%s", i, #must_publish))
        local image_nodes = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
        local image_files = {}
        for i, node in ipairs(image_nodes) do
            image_files[i] = project.source .. "/cocosstudio/" .. node:getAttributeValue("Path")
        end
        local new_file, modified, to_update = PublishRes.CheckImageState(image_files)
        PublishRes.UpdateImageState(modified)
        PublishRes.InsertImageState(new_file)
        PublishRes.TouchImageState(to_update)
    end
    if #must_publish > 0 then
        print()
    end
    PublishRes.UpdateFileState(modified)
    PublishRes.InsertFileState(new_file)
    print("plist publish " .. #must_publish .. " files")
end
