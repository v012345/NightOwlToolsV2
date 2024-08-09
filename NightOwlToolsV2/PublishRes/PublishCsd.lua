for i, project in ipairs(PublishRes.Projects) do
    local ui_directory = project.source .. "/cocosstudio/ui"
    local ui_files = PublishRes.GetFilesOfDir(ui_directory, "csd")
    local new_file, modified, to_update = PublishRes.CheckFileState(ui_files)
    local to_publish = {table.unpack(new_file)}
    table.move(modified, 1, #modified, #to_publish + 1, to_publish)
    PublishRes.UpdateModification(to_update)
    if #to_publish > 0 then
        PublishRes.PublishUi(to_publish, project.source, project.target)
    end
    PublishRes.UpdateFileState(modified)
    PublishRes.InsertFileState(new_file)
    print("csd publish " .. #to_publish .. " files")
end
