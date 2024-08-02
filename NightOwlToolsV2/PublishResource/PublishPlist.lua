local db = sqlite3.open('LocalOnly/BlackMoon.db')
for i, project in ipairs(PublishResource.Projects) do
    local cocosstudio_directory = project.from .. "/cocosstudio"
    local plist_table_name = "plist" .. Common.EasyChecksum(cocosstudio_directory)
    local plist_directory = cocosstudio_directory .. "/plist"
    local plist_current_state = PublishResource.GetStates(plist_directory, "csi", cocosstudio_directory)
    PublishResource.TouchTable(db, plist_table_name)
    local plist_last_state = PublishResource.GetLastStates(db, plist_table_name)
    local plist_to_publish, plist_maybe_publish = PublishResource.Compare(plist_current_state, plist_last_state)

    for k, plist_state in pairs(plist_maybe_publish) do
        local changed_images = PublishResource.ChangedImages(db, plist_state, cocosstudio_directory)
        if next(changed_images) then
            plist_to_publish[k] = state
        end
    end

    PublishResource.PublishPlist(plist_to_publish, project.from, project.to, cocosstudio_directory)

    for k, plist_state in pairs(plist_to_publish) do
        local changed_images = PublishResource.ChangedImages(db, plist_state, cocosstudio_directory)
        local image_table_name = "image" .. Common.EasyChecksum(plist_state.path)
        PublishResource.UpdateTable(db, image_table_name, changed_images)
    end
    PublishResource.UpdateTable(db, plist_table_name, plist_to_publish)
end
db:close()
