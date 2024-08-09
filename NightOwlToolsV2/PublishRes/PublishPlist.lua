for i, project in ipairs(PublishResource.Projects) do
    local cocosstudio_directory = project.source .. "/cocosstudio"
    local plist_directory = cocosstudio_directory .. "/plist"
    local plist_files = PublishResource.GetFilesOfDir(plist_directory, "csi")
    local new_file, modified, to_update, unchanged = PublishResource.CheckFileState(plist_files)

    local must_publish = {table.unpack(new_file)}
    table.move(modified, 1, #modified, #must_publish + 1, must_publish)

    local may_publish = {table.unpack(unchanged)}
    table.move(to_update, 1, #to_update, #may_publish + 1, may_publish)

    PublishResource.UpdateModification(to_update)

    for i, path in ipairs(may_publish) do
        Common.ShowOnOneline(string.format("double check : %s/%s", i, #may_publish))
        local image_nodes = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
        local image_files = {}
        for i, node in ipairs(image_nodes) do
            image_files[i] = project.source .. "/cocosstudio/" .. node:getAttributeValue("Path")
        end
        local new_file, modified, to_update = PublishResource.CheckImageState(image_files)
        if next(new_file) or next(modified) then
            must_publish[#must_publish + 1] = path
        else
            PublishResource.UpdateModification(to_update)
        end
    end
    if #may_publish > 0 then
        print()
    end
    if #must_publish > 0 then
        PublishResource.PublishPlist(must_publish, project.source, project.target)
    end

    for i, path in ipairs(must_publish) do
        Common.ShowOnOneline(string.format("update plist : %s/%s", i, #must_publish))
        local image_nodes = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
        local image_files = {}
        for i, node in ipairs(image_nodes) do
            image_files[i] = project.source .. "/cocosstudio/" .. node:getAttributeValue("Path")
        end
        local new_file, modified, to_update = PublishResource.CheckImageState(image_files)
        PublishResource.UpdateImageState(modified)
        PublishResource.InsertImageState(new_file)
        PublishResource.TouchImageState(to_update)
    end
    if #must_publish > 0 then
        print()
    end
    PublishResource.UpdateFileState(modified)
    PublishResource.InsertFileState(new_file)
    print("plist publish " .. #must_publish .. " files")
end
