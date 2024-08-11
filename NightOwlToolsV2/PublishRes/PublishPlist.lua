for i, project in ipairs(PublishRes.Projects) do
    local plist_dir = project.source .. "/cocosstudio/plist"
    local plist_files = Common.GetFilesOfCurDir(plist_dir, "csi")

    local created, modified, touched, unchanged = PublishRes:compareWithDB(plist_files, true)

    PublishRes:updateTouched(touched)

    local to_publish = Common.Merge(created, modified)
    local may_publish = Common.Merge(touched, unchanged)
    if #may_publish > 0 then
        local progress = string.format("double check : %%s/%s", #may_publish)
        for i, path in ipairs(may_publish) do
            Common.Write(string.format(progress, i))
            local image_nodes = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
            local image_files = {}
            for i, node in ipairs(image_nodes) do
                image_files[i] = project.source .. "/cocosstudio/" .. node:getAttributeValue("Path")
            end

            local created, modified, touched = PublishRes:compareWithDB(image_files, false)
            PublishRes:updateTouched(touched)
            if next(created) or next(modified) then
                to_publish[#to_publish + 1] = path
            end
        end
        print()
    end

    if #to_publish > 0 then
        PublishRes:PublishPlist(to_publish, project.source, project.target)
        local progress = string.format("update plist : %%s/%s", #to_publish)
        for i, path in ipairs(to_publish) do
            Common.Write(string.format(progress, i))
            local image_nodes = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
            local image_files = {}
            for i, node in ipairs(image_nodes) do
                image_files[i] = project.source .. "/cocosstudio/" .. node:getAttributeValue("Path")
            end
            local created, modified, touched = PublishRes:compareWithDB(image_files, false)
            PublishRes:UpdateFileState(modified)
            PublishRes:InsertFileState(created)
            PublishRes:updateTouched(touched)
        end
        print()
    end

    PublishRes:UpdateFileState(modified, true)
    PublishRes:InsertFileState(created)
    print(">>>>>>>>> published " .. #to_publish .. " plist files <<<<<<<<")
end
