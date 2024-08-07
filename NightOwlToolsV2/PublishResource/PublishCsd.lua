for i, project in ipairs(PublishResource.Projects) do
    local ui_directory = project.source .. "/cocosstudio/ui"
    local ui_files = PublishResource.GetFilesOfDir(ui_directory, "csd")
    local new_file, modified, to_update = PublishResource.CheckFileState(ui_files)
    local to_publish = {table.unpack(new_file)}
    table.move(modified, 1, #modified, #to_publish + 1, to_publish)
    PublishResource.UpdateModification(to_update)
    PublishResource.PublishUi(to_publish, project.source, project.target)
    PublishResource.UpdateFileState(modified)
    PublishResource.InsertFileState(new_file)
    print("csd publish " .. #to_publish .. " files")
end
