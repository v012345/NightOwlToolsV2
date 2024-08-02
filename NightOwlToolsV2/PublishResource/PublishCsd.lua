local db = sqlite3.open('LocalOnly/BlackMoon.db')
for i, project in ipairs(PublishResource.Projects) do
    local cocosstudio_directory = project.from .. "/cocosstudio"
    local ui_directory = cocosstudio_directory .. "/ui"
    local ui_table_name = "ui" .. Common.EasyChecksum(cocosstudio_directory)
    local ui_current_state = PublishResource.GetStates(ui_directory, "csd", cocosstudio_directory)
    PublishResource.TouchTable(db, ui_table_name)
    local ui_last_state = PublishResource.GetLastStates(db, ui_table_name)
    local ui_publish_state = PublishResource.Compare(ui_current_state, ui_last_state)
    PublishResource.PublishCsd(ui_publish_state, project.from, project.to)
    PublishResource.UpdateTable(db, ui_table_name, ui_publish_state)
end
db:close()
