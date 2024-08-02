local db = sqlite3.open('LocalOnly/BlackMoon.db')
for i, project in ipairs(PublishResource.Projects) do
    local cocosstudio_directory_path = project.from .. "/cocosstudio"
    local ui_table_name = "ui" .. Common.EasyChecksum(cocosstudio_directory_path)
    local ui_current_state = PublishResource.GetFilesCurrentState(cocosstudio_directory_path .. "/ui", ".csd",
        cocosstudio_directory_path)
    PublishResource.TouchTable(db, ui_table_name)
    local ui_last_state = PublishResource.GetLastStates(db, ui_table_name)
    local to_publish_ui = PublishResource.GetPublishState(ui_current_state, ui_last_state)
    PublishResource.PublishCsd(to_publish_ui, project.from, project.to, cocosstudio_directory_path)
    PublishResource.UpdateTable(db, ui_table_name, to_publish_ui)
end
db:close()
