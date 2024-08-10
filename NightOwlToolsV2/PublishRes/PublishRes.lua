require "PublishRes.config"

function PublishRes:init()
    self:initDB()
    self.DB = self:openDB()
    self:initCCS()
end

function PublishRes:initCCS()
    local file = io.open("PublishRes/ccs_template.xml", "r") or error()
    self.CCS = file:read("a")
    file:close()
end

function PublishRes:initDB()
    local db = sqlite3.open(self.DBPath)
    local query = "SELECT name FROM sqlite_master WHERE type='table' AND name='%s'"
    local iterator, tables = db:nrows(string.format(query, self.TableName))
    -- if not exist, create it
    if not iterator(tables) then
        query = "CREATE TABLE %s (path TEXT PRIMARY KEY, modification TIMESTAMP,checksum TEXT);"
        db:exec(string.format(query, self.TableName))
    end
    db:close()
end

function PublishRes:openDB()
    local db = sqlite3.open_memory()
    local query = "ATTACH DATABASE '%s' AS disk_db"
    assert(db:exec(string.format(query, self.DBPath)) == sqlite3.OK)
    query = "CREATE TABLE %s AS SELECT * FROM disk_db.%s"
    assert(db:exec(string.format(query, self.TableName, self.TableName)) == sqlite3.OK)
    assert(db:exec("DETACH DATABASE disk_db") == sqlite3.OK)
    return db
end

function PublishRes:realse()
    -- 这里会不会先把 sqlite3 给收回了, 再来回收这个?
    local db = self.DB
    local query = "ATTACH DATABASE '%s' AS publis_res_disk_db"
    assert(db:exec(string.format(query, self.DBPath)) == sqlite3.OK)
    assert(db:exec("DELETE FROM publis_res_disk_db.file_states") == sqlite3.OK)
    query = "INSERT INTO publis_res_disk_db.%s SELECT * FROM %s"
    assert(db:exec(string.format(query, self.TableName, self.TableName)) == sqlite3.OK)
    assert(db:exec("DETACH DATABASE publis_res_disk_db") == sqlite3.OK)
    db:close()
end

function PublishRes:getStateByPath(path)
    local query = "SELECT * FROM %s WHERE path = ?"
    local stmt = self.DB:prepare(string.format(query, self.TableName))
    stmt:bind_values(path)
    local result = stmt:step()
    local row = nil
    if result == sqlite3.ROW then
        row = stmt:get_named_values()
    end
    stmt:reset()
    stmt:finalize()
    return row
end

function PublishRes:InsertFileState(to_insert_files)
    local to_update_num = #to_insert_files
    local querry = "INSERT INTO %s(path,modification,checksum) VALUES('%%s',%%s,'%%s');"
    local querry = string.format(querry, self.TableName)
    local queries = {}
    for i, path in ipairs(to_insert_files) do
        Common.Write(string.format("calculate checksum : %s/%s", i, to_update_num))
        local modification = lfs.attributes(path, "modification")
        local checksum = Common.Checksum(path)
        queries[i] = string.format(querry, path, modification, checksum)
    end

    if to_update_num > 0 then
        print()
        self.DB:exec(table.concat(queries))
        print("insert state : " .. to_update_num)
    end
end

function PublishRes.UpdateFileState(to_update_files)
    local to_update_num = #to_update_files
    local update_query = "UPDATE file_state SET modification = %s,checksum = '%s' WHERE path = '%s';"
    local queries = {}
    for i, path in ipairs(to_update_files) do
        local modification = lfs.attributes(path, "modification")
        local checksum = Common.Checksum(path)
        queries[i] = string.format(update_query, modification, checksum, path)
    end
    if to_update_num > 0 then
        update_query = table.concat(queries)
        self.DB:exec(update_query)
        print("update state : " .. to_update_num)
    end
end

function PublishRes:updateTouched(touched, is_show_progress)
    local total = #touched
    local query = string.format("UPDATE %s SET modification = %%s WHERE path = '%%s';", self.TableName)
    local queries = {}
    for i, path in ipairs(touched) do
        local modification = lfs.attributes(path, "modification")
        queries[i] = string.format(query, modification, path)
    end
    if is_show_progress and total > 0 then
        print("touch  state : " .. total)
        self.DB:exec(table.concat(queries))
    end
end

function PublishRes:compareWithDB(paths, is_show_progress)
    local created, modified, touched, unchanged = {}, {}, {}, {}
    local total = #paths
    for i, path in ipairs(paths) do
        local db_file_state = self:getStateByPath(path)
        if db_file_state then
            local modification = lfs.attributes(path, "modification")
            if db_file_state.modification < modification then
                local checksum = Common.Checksum(path)
                if checksum ~= db_file_state.checksum then
                    table.insert(modified, path)
                else
                    table.insert(touched, path)
                end
            else
                table.insert(unchanged, path)
            end
        else
            table.insert(created, path)
        end

        if is_show_progress then
            Common.Write(string.format("check  files : %s/%s", i, total))
        end
    end
    if is_show_progress and total > 0 then
        print()
    end
    return created, modified, touched, unchanged
end

function PublishRes:CheckImageState(paths)
    local new = {}
    local modified = {}
    local touched = {}
    local unchanged = {}
    local total = #paths
    for i, path in ipairs(paths) do
        local old = self:getStateByPath(path)
        if old then
            local modification = lfs.attributes(path, "modification")
            if old.modification < modification then
                local checksum = Common.Checksum(path)
                if checksum ~= old.checksum then
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
    local update_query = "UPDATE file_state SET modification = %s,checksum = '%s' WHERE path = '%s';"
    local queries = {}
    for i, path in ipairs(to_update_files) do
        local modification = lfs.attributes(path, "modification")
        local checksum = Common.Checksum(path)
        queries[i] = string.format(update_query, modification, checksum, path)
    end
    if to_update_num > 0 then
        update_query = table.concat(queries)
        PublishRes.mem_db:exec(update_query)
    end
end

function PublishRes.InsertImageState(to_insert_files)
    local to_update_num = #to_insert_files
    local update_query = " INSERT INTO file_state(path,modification,checksum) VALUES('%s',%s,'%s');"
    local queries = {}
    for i, path in ipairs(to_insert_files) do
        local modification = lfs.attributes(path, "modification")
        local checksum = Common.Checksum(path)
        queries[i] = string.format(update_query, path, modification, checksum)
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

function PublishRes:publishUi(to_publish, source, target)
    if #to_publish <= 0 then
        return
    end

    local file_names = {}
    local ui_dir = source .. "/cocosstudio/ui/"
    for i, path in ipairs(to_publish) do
        file_names[i] = string.gsub(path, ui_dir, "", 1)
    end

    ---@type XML
    local ccs = XML(self.CCS)
    local root = ccs:getRootNode()
    local ui_node = root:getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "ui") or error()
    for _, name in ipairs(file_names) do
        ---@type XMLNode
        local node = XML:newNode("Project")
        node:setAttributeValue("Name", name)
        node:setAttributeValue("Type", "Layer")
        ui_node:addChild(node)
    end
    local temp = source .. "/.temp.ccs"
    ccs:writeTo(temp)
    if #to_publish > 0 then
        self:publish(temp, target)
    end
    os.remove(temp)
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

function PublishRes:publish(css, target)
    local start_time = os.time()
    Common.Write("start publish, please wait")

    local cmd = 'cmd /c ""%s" publish -f "%s" -o "%s" -s -d Serializer_FlatBuffers"'
    local CocosTool = string.gsub(self.CocosTool, "\\", "\\\\")

    local publist_cmd = string.format(cmd, CocosTool, css, target)
    local handle = Common.CreateThread(string.format([[
    local publist_cmd ='%s'
    local shell = io.popen(publist_cmd) or error("can't execute " .. publist_cmd)
    result = shell:read("a")
    shell:close()
    return result
    ]], publist_cmd))
    local i = 0
    while not Common.WaitForSingleObject(handle) do
        Common.Write(string.format("Publishing %ss", i))
        i = i + 1
        Common.sleep(1)
    end
    print()
    local result = Common.GetExitCodeThread(handle)
    Common.CloseHandle(handle)
    if string.find(result, "Publish success!") then
        Common.Write(string.format("Publish success! spent %ss", os.time() - start_time))
    else
        error(result)
    end
end

PublishRes:init()
setmetatable(PublishRes, {
    __gc = PublishRes.realse
});
return PublishRes
