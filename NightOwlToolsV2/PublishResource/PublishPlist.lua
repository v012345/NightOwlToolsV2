local db = sqlite3.open('LocalOnly/BlackMoon.db')
for i, project in ipairs(PublishResource.Projects) do
    local root_folder = project.from .. "/cocosstudio"
    local plist_table_name = "plist" .. Common.EasyChecksum(root_folder)
    local plist_folder = root_folder .. "/plist"
    local plist_current_state = PublishResource.GetFilesCurrentState(plist_folder, ".csi", root_folder)
    PublishResource.TouchTable(db, plist_table_name)
    local plist_last_state = PublishResource.GetLastStates(db, plist_table_name)
    local to_publish_plist, maybe_publish_plist = PublishResource.GetPublishState(plist_current_state, plist_last_state)

    -- print("--------------")
    -- for k, v in pairs(maybe_publish_plist) do
    --     print(v.name)
    -- end

    for k, v in pairs(maybe_publish_plist) do
        local changed_images = PublishResource.GetChangedImageOfPlist(db, v, plist_table_name .. Common.EasyChecksum(k),
            root_folder)
        if next(changed_images) then
            to_publish_plist[k] = v
        end
    end

    -- finshed

    for k, v in pairs(to_publish_plist) do
        local changed_images = PublishResource.GetChangedImageOfPlist(db, v, plist_table_name .. Common.EasyChecksum(k),
            root_folder)
        PublishResource.UpdateTable(db, plist_table_name .. Common.EasyChecksum(k), changed_images)
    end
    PublishResource.UpdateTable(db, plist_table_name, to_publish_plist)
    -- local toPublish = {}
    -- local lastState = {}
    -- local nowState = {}
    -- for i, v in ipairs(csi_files) do
    --     local relative_path = string.sub(v.path, uiRootLen)
    --     nowState[relative_path] = {
    --         name = v.name,
    --         modification = v.modification,
    --         relative_path = relative_path,
    --         path = v.path
    --     }
    -- end
    -- for row in db:nrows("SELECT * FROM " .. plistTableNmae) do
    --     lastState[row.relative_path] = {
    --         id = row.id,
    --         name = row.name,
    --         sha1 = row.sha1,
    --         modification = row.modification,
    --         relative_path = row.relative_path,
    --         path = uiRoot .. "/" .. row.relative_path
    --     }
    -- end

    -- local function isImageChange(nowState, RootFolder, db)
    --     local hash = Common.EasyChecksum(RootFolder)
    --     for k, v in pairs(nowState) do
    --         local csi_images_table = string.sub(v.name, 1, #v.name - 4) .. hash
    --         PublishResource.TouchTable(db, csi_images_table)
    --         local imageLastStates = PublishResource.GetLastStates(db, csi_images_table, RootFolder .. "/cocosstudio/")
    --         local stmt = PublishResource.CreateSmart(db, csi_images_table)
    --         -- print(csi_images_table)

    --         -- ---@type XMLNode[]
    --         local ImageFiles = XML(v.path):getRootNode():getChild(2):getChild(1):getChildren()
    --         for i, v in ipairs(ImageFiles) do
    --             local relative_path = v:getAttributeValue("Path")
    --             Common.ShowOnOneline(string.format("update db %s/%s %s", i, #ImageFiles, relative_path))
    --             local path = RootFolder .. "/cocosstudio/" .. relative_path
    --             if imageLastStates[relative_path] then
    --                 local last = imageLastStates[relative_path]
    --                 if (last.modification < lfs.attributes(path, "modification") and last.sha1 ~= Win32.getSha1(path)) then
    --                     stmt:bind_names({
    --                         id = v.id,
    --                         modification = 1,
    --                         sha1 = 1,
    --                         name = relative_path,
    --                         relative_path = relative_path
    --                     })
    --                 end
    --             else
    --                 stmt:bind_names({
    --                     id = v.id,
    --                     modification = 1,
    --                     sha1 = 1,
    --                     name = relative_path,
    --                     relative_path = relative_path
    --                 })
    --             end
    --             stmt:step()
    --             stmt:reset()
    --         end
    --         stmt:finalize()
    --     end
    -- end
    -- isImageChange(nowState, RootFolder, db)
    -- do
    --     return
    -- end
    -- local function updateImageState()

    -- end

    -- for k, v in pairs(nowState) do
    --     local last = lastState[k]
    --     if last then
    --         if (last.modification < v.modification and last.sha1 ~= Win32.getSha1(v.path)) or PublishResource.PublishAll then
    --             toPublish[#toPublish + 1] = last
    --         elseif isImageChange() then
    --             toPublish[#toPublish + 1] = last
    --         end
    --     else
    --         toPublish[#toPublish + 1] = v
    --     end
    -- end

    -- local publishNum = #toPublish
    -- if publishNum > 0 then
    --     local stmt = db:prepare(string.format([[ INSERT OR REPLACE INTO 
    --               %s (id,modification,sha1,name,relative_path)
    --               VALUES (:id,:modification,:sha1,:name,:relative_path)]], plistTableNmae))

    --     for i, v in ipairs(toPublish) do
    --         Common.ShowOnOneline(string.format("update db %s/%s %s", i, publishNum, v.name))
    --         stmt:bind_names({
    --             id = v.id,
    --             modification = v.modification,
    --             sha1 = Win32.getSha1(v.path),
    --             name = v.name,
    --             relative_path = v.relative_path
    --         })
    --         stmt:step()
    --         stmt:reset()
    --     end
    --     stmt:finalize()
    --     print()
    --     ---@type XML
    --     local temp_css = XML [[
    -- <Solution>
    --     <PropertyGroup Name="Closers" Version="3.10.0.0" Type="CocosStudio" />
    --     <SolutionFolder>
    --         <Group ctype="ResourceGroup">
    --             <RootFolder Name=".">
    --                 <Folder Name="plist">
    --                 </Folder>
    --             </RootFolder>
    --         </Group>
    --     </SolutionFolder>
    -- </Solution>
    -- ]]
    --     ---@type XMLNode
    --     local ui_node = temp_css:getRootNode():getChild(2):getChild(1):getChild(1):getChild(1)
    --     --   <PlistInfo Name="ui_zhandouwenzi.csi" Type="Plist" />
    --     for i, v in ipairs(toPublish) do
    --         ---@type XMLNode
    --         local newNode = XML:newNode("PlistInfo")
    --         newNode:setAttributeValue("Name", v.name)
    --         newNode:setAttributeValue("Type", "Plist")
    --         ui_node:addChild(newNode)
    --     end
    --     local temp_css_file = RootFolder .. "/Closers_temp1.ccs"
    --     temp_css:writeTo(temp_css_file)

    --     local cocos_cmd = '"%s" publish -f %s -o %s -s -d Serializer_FlatBuffers'
    --     local cmd = string.format(cocos_cmd, PublishResource.CocosTool, temp_css_file, RootFolders.to)
    --     local exe_cmd = io.popen(cmd) or error("can't execute " .. cocos_cmd)
    --     print(exe_cmd:read("a"))
    --     exe_cmd:close()
    --     os.remove(temp_css_file)
    -- else
    --     print(string.format("update %s files", publishNum))
    -- end

end
db:close()
