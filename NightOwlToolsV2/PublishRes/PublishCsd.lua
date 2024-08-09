for i, project in ipairs(PublishRes.Projects) do
    local ui_dir = project.source .. "/cocosstudio/ui"
    local ui_files = Common.GetFilesOfCurDir(ui_dir, "csd")
    local created, modified, touched = PublishRes:compareWithDB(ui_files, true)
    PublishRes:updateTouched(touched, true)
    local to_publish = Common.Merge(created, modified)
    PublishRes:publishUi(to_publish, project.source, project.target)
    -- PublishRes.UpdateFileState(modified)
    -- PublishRes.InsertFileState(new_file)
    -- print("csd publish " .. #to_publish .. " files")
end
