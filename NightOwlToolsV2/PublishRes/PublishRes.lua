function PublishRes:init()
    local disk_db = sqlite3.open('LocalOnly/PublishRes.db')
    local query = "SELECT name FROM sqlite_master WHERE type='table' AND name='file_state'"
    local iterator, sqlite_vm = disk_db:nrows(query)
    if not iterator(sqlite_vm) then
        query = "CREATE TABLE file_state (path TEXT PRIMARY KEY, modification TIMESTAMP,md5 TEXT);"
        disk_db:exec(query)
    end
    disk_db:close()
    self.mem_db = sqlite3.open_memory()
    assert(self.mem_db:exec("ATTACH DATABASE 'LocalOnly/PublishRes.db' AS disk_db") == sqlite3.OK)
    assert(self.mem_db:exec("CREATE TABLE file_state AS SELECT * FROM disk_db.file_state") == sqlite3.OK)
    assert(self.mem_db:exec("DETACH DATABASE disk_db") == sqlite3.OK)

    local stmt = self.mem_db:prepare("SELECT * FROM file_state WHERE path = ?")
    self.GetInfoByPath = function(path)
        stmt:bind_values(path)
        local result = stmt:step()
        local row = nil
        if result == sqlite3.ROW then
            row = stmt:get_named_values()
        end
        stmt:reset()
        return row
    end
end

function PublishRes:realse()
    assert(self.mem_db:exec("ATTACH DATABASE 'LocalOnly/PublishRes.db' AS disk_db") == sqlite3.OK)
    assert(self.mem_db:exec("DELETE FROM disk_db.file_state") == sqlite3.OK)
    if self.mem_db:exec("INSERT INTO disk_db.file_state SELECT * FROM file_state") ~= sqlite3.OK then
        print(self.mem_db:errmsg())
    end
    -- assert(self.mem_db:exec("INSERT INTO disk_db.file_state SELECT * FROM file_state") == sqlite3.OK)
    assert(self.mem_db:exec("DETACH DATABASE disk_db") == sqlite3.OK)
    self.mem_db:close()
end

function PublishRes.InsertFileState(to_insert_files)
    local to_update_num = #to_insert_files
    local update_query = " INSERT INTO file_state(path,modification,md5) VALUES('%s',%s,'%s');"
    local queries = {}
    for i, path in ipairs(to_insert_files) do
        Common.ShowOnOneline(string.format("calculate md5 : %s/%s", i, to_update_num))
        local modification = lfs.attributes(path, "modification")
        local md5 = Common.Md5(path)
        queries[i] = string.format(update_query, path, modification, md5)
    end

    if to_update_num > 0 then
        print()
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
        print("insert state : " .. to_update_num)
    end
end

function PublishRes.UpdateFileState(to_update_files)
    local to_update_num = #to_update_files
    local update_query = "UPDATE file_state SET modification = %s,md5 = '%s' WHERE path = '%s';"
    local queries = {}
    for i, path in ipairs(to_update_files) do
        local modification = lfs.attributes(path, "modification")
        local md5 = Common.Md5(path)
        queries[i] = string.format(update_query, modification, md5, path)
    end
    if to_update_num > 0 then
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
        print("update state : " .. to_update_num)
    end
end

function PublishRes.UpdateModification(to_update_files)
    local to_update_num = #to_update_files
    local update_query = "UPDATE file_state SET modification = %s WHERE path = '%s';"
    local queries = {}
    for i, path in ipairs(to_update_files) do
        local modification = lfs.attributes(path, "modification")
        queries[i] = string.format(update_query, modification, path)
    end
    if to_update_num > 0 then
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
        print("touch  state : " .. to_update_num)
    end
end

function PublishRes.CheckFileState(paths)
    local new = {}
    local modified = {}
    local touched = {}
    local unchanged = {}
    local total = #paths
    for i, path in ipairs(paths) do
        Common.ShowOnOneline(string.format("check  files : %s/%s", i, total))
        local old = PublishRes.GetInfoByPath(path)
        if old then
            local modification = lfs.attributes(path, "modification")
            if old.modification < modification then
                local md5 = Common.Md5(path)
                if md5 ~= old.md5 then
                    table.insert(modified, path)
                else
                    table.insert(touched, path)
                end
            else
                table.insert(unchanged, path)
            end
        else
            table.insert(new, path)
        end
    end
    if total > 0 then
        print()
    end
    return new, modified, touched, unchanged
end

function PublishRes.CheckImageState(paths)
    local new = {}
    local modified = {}
    local touched = {}
    local unchanged = {}
    local total = #paths
    for i, path in ipairs(paths) do
        local old = PublishRes.GetInfoByPath(path)
        if old then
            local modification = lfs.attributes(path, "modification")
            if old.modification < modification then
                local md5 = Common.Md5(path)
                if md5 ~= old.md5 then
                    table.insert(modified, path)
                else
                    table.insert(touched, path)
                end
            else
                table.insert(unchanged, path)
            end
        else
            table.insert(new, path)
        end
    end
    return new, modified, touched, unchanged
end

function PublishRes.UpdateImageState(to_update_files)
    local to_update_num = #to_update_files
    local update_query = "UPDATE file_state SET modification = %s,md5 = '%s' WHERE path = '%s';"
    local queries = {}
    for i, path in ipairs(to_update_files) do
        local modification = lfs.attributes(path, "modification")
        local md5 = Common.Md5(path)
        queries[i] = string.format(update_query, modification, md5, path)
    end
    if to_update_num > 0 then
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
    end
end

function PublishRes.InsertImageState(to_insert_files)
    local to_update_num = #to_insert_files
    local update_query = " INSERT INTO file_state(path,modification,md5) VALUES('%s',%s,'%s');"
    local queries = {}
    for i, path in ipairs(to_insert_files) do
        local modification = lfs.attributes(path, "modification")
        local md5 = Common.Md5(path)
        queries[i] = string.format(update_query, path, modification, md5)
    end

    if to_update_num > 0 then
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
    end
end

function PublishRes.TouchImageState(to_update_files)
    local to_update_num = #to_update_files
    local update_query = "UPDATE file_state SET modification = %s WHERE path = '%s';"
    local queries = {}
    for i, path in ipairs(to_update_files) do
        local modification = lfs.attributes(path, "modification")
        queries[i] = string.format(update_query, modification, path)
    end
    if to_update_num > 0 then
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
    end
end

function PublishRes.GetFilesOfDir(folder, suffix)
    suffix = string.lower(suffix)
    local pattern = "^.+%." .. suffix .. "$"
    local files = {}
    for entry in lfs.dir(folder) do
        local filePath = folder .. "/" .. entry
        local file_attributes = lfs.attributes(filePath)
        -- if file_attributes.mode == "file" then
        if string.match(string.lower(filePath), pattern) then
            files[#files + 1] = filePath
        end
        -- end
    end
    return files
end

function PublishRes.PublishUi(to_publish, source, target)
    local to_publish_file = {}
    local ui_directory = source .. "/cocosstudio/ui/"
    for i, path in ipairs(to_publish) do
        to_publish_file[i] = string.gsub(path, ui_directory, "", 1)
    end

    ---@type XML
    local css_file_template = XML(PublishRes.CCS_Template)
    local root_node = css_file_template:getRootNode()
    ---@type XMLNode
    local Folder_node = root_node:getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "ui")
    for _, name in ipairs(to_publish_file) do
        ---@type XMLNode
        local newNode = XML:newNode("Project")
        newNode:setAttributeValue("Name", name)
        newNode:setAttributeValue("Type", "Layer")
        Folder_node:addChild(newNode)
    end
    local temp_css_file = source .. "/temp_css_file.ccs"
    css_file_template:writeTo(temp_css_file)
    if #to_publish_file > 0 then
        PublishRes.StartPublish(temp_css_file, target)
    end
    os.remove(temp_css_file)
end

function PublishRes.PublishPlist(to_publish, source, target)
    ---@type XML
    local css_file_template = XML(PublishRes.CCS_Template)
    local root_node = css_file_template:getRootNode()
    ---@type XMLNode
    local RootFolder_node = root_node:getChild(2):getChild(1):getChild(1)
    local plist_node = root_node:getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "plist")

    local plist_directory = source .. "/cocosstudio/plist/"
    local cocosstudio_directory = source .. "/cocosstudio"
    for _, path in pairs(to_publish) do
        local PlistInfo_node = XML:newNode("PlistInfo")
        PlistInfo_node:setAttributeValue("Name", string.gsub(path, plist_directory, "", 1))
        PlistInfo_node:setAttributeValue("Type", "Plist")
        plist_node:addChild(PlistInfo_node)

        local ImageFiles_node = XML(path):getRootNode():getChild(2):getChild(1):getChildren()
        for i, image_node in ipairs(ImageFiles_node) do
            local rp = image_node:getAttributeValue("Path")
            local temp_node = RootFolder_node
            local temp_directory = cocosstudio_directory
            for k, v in pairs(Common.Split(rp, "/")) do
                temp_directory = temp_directory .. "/" .. v
                local file_attributes = lfs.attributes(temp_directory)
                if file_attributes.mode == "directory" then
                    local Folder_node = temp_node:getChildByAttri("Name", v)
                    if not Folder_node then
                        Folder_node = XML:newNode("Folder")
                        Folder_node:setAttributeValue("Name", v)
                        temp_node:addChild(Folder_node)
                    end
                    temp_node = Folder_node
                elseif file_attributes.mode == "file" then
                    local Image_node = temp_node:getChildByAttri("Name", v)
                    if not Image_node then
                        Image_node = XML:newNode("Image")
                        Image_node:setAttributeValue("Name", v)
                        temp_node:addChild(Image_node)
                    end
                end
            end
        end
    end
    if #to_publish > 0 then
        local temp_css_file = source .. "/temp_css_file.ccs"
        css_file_template:writeTo(temp_css_file)
        PublishRes.StartPublish(temp_css_file, target)
        os.remove(temp_css_file)
    end
end

function PublishRes.StartPublish(css_file, publish_directory)
    local start_time = os.time()
    Common.ShowOnOneline("start publish, please wait")
    local cocos_cmd = '"%s" publish -f %s -o %s -s -d Serializer_FlatBuffers'
    local cmd = string.format(cocos_cmd, PublishRes.CocosTool, css_file, publish_directory)
    local exe_cmd = io.popen(cmd) or error("can't execute " .. cocos_cmd)
    local result = exe_cmd:read("a")
    exe_cmd:close()
    if string.find(result, "Publish success!") then
        Common.ShowOnOneline(string.format("Publish success! spent %ss", os.time() - start_time))
    else
        error(result)
    end
    print()
end
