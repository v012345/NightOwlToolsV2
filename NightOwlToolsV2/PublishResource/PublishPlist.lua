local db = sqlite3.open('LocalOnly/BlackMoon.db')
for i, project in ipairs(PublishResource.Projects) do
    local cocosstudio_directory_path = project.from .. "/cocosstudio"
    local plist_table_name = "plist" .. Common.EasyChecksum(cocosstudio_directory_path)
    local plist_current_state = PublishResource.GetFilesCurrentState(cocosstudio_directory_path .. "/plist", ".csi",
        cocosstudio_directory_path)
    PublishResource.TouchTable(db, plist_table_name)
    local plist_last_state = PublishResource.GetLastStates(db, plist_table_name)
    local to_publish_plist, maybe_publish_plist = PublishResource.GetPublishState(plist_current_state, plist_last_state)

    for k, v in pairs(maybe_publish_plist) do
        local images_table_name = plist_table_name .. Common.EasyChecksum(k)
        local changed_images = PublishResource.GetChangedImageOfPlist(db, v, images_table_name,
            cocosstudio_directory_path)
        if next(changed_images) then
            to_publish_plist[k] = v
        end
    end

    PublishResource.PublishPlist(to_publish_plist, project.from, project.to, cocosstudio_directory_path)

    for k, v in pairs(to_publish_plist) do
        local images_table_name = plist_table_name .. Common.EasyChecksum(k)
        local changed_images = PublishResource.GetChangedImageOfPlist(db, v, images_table_name,
            cocosstudio_directory_path)
        PublishResource.UpdateTable(db, plist_table_name .. Common.EasyChecksum(k), changed_images)
    end
    PublishResource.UpdateTable(db, plist_table_name, to_publish_plist)
end
db:close()
