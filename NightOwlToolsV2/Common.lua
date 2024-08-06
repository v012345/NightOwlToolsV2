require "Tools.XML"
require "Tools.CSV"

Common = {}
function Common.ShowOnOneline(out)
    local out = "\r" .. out
    local curLen = #out
    out = out .. string.rep(" ", 100 - curLen)
    io.write(out)
end

function Common.GetMapItemNum(map)
    local total = 0
    for k, v in pairs(map) do
        total = total + 1
    end
    return total
end

function Common.Split(inputstr, sep)
    if sep == nil then
        sep = "%s"
    end
    local t = {}
    for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
        table.insert(t, str)
    end
    return t
end

function Common.EasyChecksum(str)
    local l = #str
    local h = 83600 ~ l
    for i = l, 1, -1 do
        h = h ~ ((h << 5) + (h >> 2) + string.byte(str, i, i))
    end
    return string.format("%x", h)
end

PublishResource = {
    CocosTool = "C:\\Cocos\\Cocos Studio\\Cocos.Tool.exe",
    DB_Path = "LocalOnly/PublishResource.db",
    PublishAll = false, -- 全部发布
    Projects = {        --
        {
            source = "D:/Closers.cocos/resource/ui/branches/qooapp/zhtw",
            target = "D:/Closers.cocos/client/branches/qooapp/Resources/res_zhtw"
            -- to = "D:/NightOwlToolsV2/NightOwlToolsV2/LocalOnly"
        } --
        -- {
        --     from = "D:/Closers.cocos/resource/ui/branches/dzogame_sea/zhcn",
        --     to = "D:/Closers.cocos/client/branches/dzogame_sea/Resources/res_zhcn"
        -- }, --
        -- {
        --     from = "D:/Closers.cocos/resource/ui/branches/online",
        --     to = "D:/Closers.cocos/client/branches/online/Resources/res"
        -- } --
    },
    CCS_Template = [[
<Solution>
	<PropertyGroup Name="Closers" Version="3.10.0.0" Type="CocosStudio" />
	<SolutionFolder>
		<Group ctype="ResourceGroup">
			<RootFolder Name=".">
				<Folder Name="image"></Folder>
				<Folder Name="piece"></Folder>
				<Folder Name="plist"></Folder>
				<Folder Name="ui"></Folder>
			</RootFolder>
		</Group>
	</SolutionFolder>
</Solution>
]]
}

function PublishResource:init()
    local DB = sqlite3.open(self.DB_Path)
    local iterator, sqlite_vm = DB:nrows("SELECT name FROM sqlite_master WHERE type='table' AND name='FileStates';")
    if not iterator(sqlite_vm) then
        DB:exec [[
            CREATE TABLE FileStates (
                path PRIMARY KEY,
                modification TIMESTAMP,
                sha1 TEXT )
            ]]
    end
    local stmt = DB:prepare [[
            INSERT OR REPLACE INTO FileStates
            (modification,sha1,path)
            VALUES (:modification,:sha1,:path)
    ]]

    self.UpdateOrInsert = function(file_state)
        stmt:bind_names(file_state)
        stmt:step()
        stmt:reset()
    end

    local stmt = DB:prepare("SELECT * FROM FileStates WHERE path = ?")
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

PublishResource:init()

function PublishResource.CheckModification(paths)
    local modified = {}
    local unchanged = {}
    local touched = {}
    for _, path in ipairs(paths) do
        local old = PublishResource.GetInfoByPath(path)
        if old then
            local modification = lfs.attributes(path, "modification")
            if old.modification < modification then
                Common.ShowOnOneline("checksum " .. path)
                local sha1 = Win32.getSha1(path)
                if sha1 ~= old.sha1 then
                    print(old.sha1)
                    print(sha1)
                    table.insert(modified, path)
                else
                    table.insert(unchanged, path)
                    table.insert(touched, path)
                end
            else
                table.insert(unchanged, path)
            end
        else
            table.insert(modified, path)
        end
    end
    return modified, unchanged, touched
end

function PublishResource.GetFilesOfDir(folder, suffix)
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

function PublishResource.PublishUi(states_will_publish, css_file_place_path, publish_directory)
    ---@type XML
    local css_file_template = XML(PublishResource.CCS_Template)
    local root_node = css_file_template:getRootNode()
    ---@type XMLNode
    local Folder_node = root_node:getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "ui")
    for _, name in ipairs(states_will_publish) do
        ---@type XMLNode
        local newNode = XML:newNode("Project")
        newNode:setAttributeValue("Name", name)
        newNode:setAttributeValue("Type", "Layer")
        Folder_node:addChild(newNode)
    end
    local temp_css_file = css_file_place_path .. "/temp_css_file.ccs"
    css_file_template:writeTo(temp_css_file)
    if Common.GetMapItemNum(states_will_publish) > 0 then
        PublishResource.StartPublish(temp_css_file, publish_directory)
    end
    os.remove(temp_css_file)
end

function PublishResource.PublishPlist(states_will_publish, css_file_place_path, publish_directory, cocosstudio_directory)
    ---@type XML
    local css_file_template = XML(PublishResource.CCS_Template)
    local root_node = css_file_template:getRootNode()
    ---@type XMLNode
    local RootFolder_node = root_node:getChild(2):getChild(1):getChild(1)
    local plist_node = root_node:getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "plist")

    for _, plist_state in pairs(states_will_publish) do
        local PlistInfo_node = XML:newNode("PlistInfo")
        PlistInfo_node:setAttributeValue("Name", plist_state.name)
        PlistInfo_node:setAttributeValue("Type", "Plist")
        plist_node:addChild(PlistInfo_node)

        local ImageFiles_node = XML(plist_state.path):getRootNode():getChild(2):getChild(1):getChildren()
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

    local temp_css_file = css_file_place_path .. "/temp_css_file.ccs"
    css_file_template:writeTo(temp_css_file)
    if Common.GetMapItemNum(states_will_publish) > 0 then
        PublishResource.StartPublish(temp_css_file, publish_directory)
    end
    os.remove(temp_css_file)
end

function PublishResource.PublishCsd(states_will_publish, css_file_place_path, publish_directory)
    ---@type XML
    local css_file_template = XML(PublishResource.CCS_Template)
    local root_node = css_file_template:getRootNode()
    ---@type XMLNode
    local Folder_node = root_node:getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "ui")
    for i, v in pairs(states_will_publish) do
        ---@type XMLNode
        local newNode = XML:newNode("Project")
        newNode:setAttributeValue("Name", v.name)
        newNode:setAttributeValue("Type", "Layer")
        Folder_node:addChild(newNode)
    end
    local temp_css_file = css_file_place_path .. "/temp_css_file.ccs"
    css_file_template:writeTo(temp_css_file)
    if Common.GetMapItemNum(states_will_publish) > 0 then
        PublishResource.StartPublish(temp_css_file, publish_directory)
    end
    os.remove(temp_css_file)
end

function PublishResource.StartPublish(css_file, publish_directory)
    local start_time = os.time()
    Common.ShowOnOneline("start publish, please wait")
    local cocos_cmd = '"%s" publish -f %s -o %s -s -d Serializer_FlatBuffers'
    local cmd = string.format(cocos_cmd, PublishResource.CocosTool, css_file, publish_directory)
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

---@param db userdata
---@param table_name string
---@param states state[]
function PublishResource.UpdateTable(db, table_name, states)
    local stmt = PublishResource.CreateSmart(db, table_name)

    local total = Common.GetMapItemNum(states)
    local i = 1
    for k, v in pairs(states) do
        Common.ShowOnOneline(string.format("update db %s/%s %s", i, total, v.name))
        stmt:bind_names({
            id = v.id,
            modification = v.modification,
            sha1 = Win32.getSha1(v.path),
            name = v.name,
            relative_path = v.relative_path,
            path = v.path
        })
        stmt:step()
        stmt:reset()
        i = i + 1
    end
    if i == 1 then
        print("update db 0/0")
    else
        print()
    end
    stmt:finalize()
end

function PublishResource.ChangedImages(db, plist_state, cocosstudio_directory)
    if string.sub(cocosstudio_directory, #cocosstudio_directory, #cocosstudio_directory) ~= "/" then
        cocosstudio_directory = cocosstudio_directory .. "/"
    end
    local image_states = {}
    local image_table_name = "image" .. Common.EasyChecksum(plist_state.path)
    ---@type XMLNode[]
    local ImageFiles = XML(plist_state.path):getRootNode():getChild(2):getChild(1):getChildren()
    for i, v in ipairs(ImageFiles) do
        local rp = v:getAttributeValue("Path")
        image_states[rp] = {
            path = cocosstudio_directory .. rp,
            name = rp,
            modification = lfs.attributes(cocosstudio_directory .. rp, "modification"),
            relative_path = rp
        }
    end
    PublishResource.TouchTable(db, image_table_name)
    local image_last_state = PublishResource.GetLastStates(db, image_table_name)
    return (PublishResource.Compare(image_states, image_last_state))
end

---@param now state[]
---@param last state[]
---@return state[],state[]
function PublishResource.Compare(states_now, states_last)
    local states_have_changed = {}
    local states_not_changed = {}
    for k, state_now in pairs(states_now) do
        local state_old = states_last[k]
        if state_old then
            state_now.id = state_old.id
            if state_old.modification < state_now.modification and Win32.getSha1(state_now.path) ~= state_old.sha1 then
                states_have_changed[k] = state_now
            else
                states_not_changed[k] = state_now
            end
        else
            states_have_changed[k] = state_now
        end
    end
    return states_have_changed, states_not_changed
end

---@return state[]
function PublishResource.GetLastStates(db, tableName)
    local last_states = {}
    for row in db:nrows("SELECT * FROM " .. tableName) do
        last_states[row.relative_path] = {
            id = row.id,
            name = row.name,
            sha1 = row.sha1,
            modification = row.modification,
            relative_path = row.relative_path,
            path = row.path
        }
    end
    return last_states
end

function PublishResource.CreateSmart(db, tableName)
    return db:prepare(string.format(
        "INSERT OR REPLACE INTO %s (id,modification,sha1,name,relative_path,path) VALUES (:id,:modification,:sha1,:name,:relative_path,:path)",
        tableName))
end

function PublishResource.TouchTable(db, tableName)
    return db:exec(string.format([[
        CREATE TABLE %s (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            modification TIMESTAMP,
            sha1 TEXT,
            name TEXT,
            path TEXT,
            relative_path TEXT
        );
]], tableName))
end

--- func desc
---@param folder string
---@param suffix string
---@param root_folder string
---@return state[]
function PublishResource.GetStates(folder, suffix, root_folder)
    if string.sub(root_folder, #root_folder, #root_folder) ~= "/" then
        root_folder = root_folder .. "/"
    end
    local start_index = #root_folder + 1
    suffix = string.lower(suffix)
    ---@type state[]
    local states = {}
    local pattern = "^.+%." .. suffix .. "$"
    for entry in lfs.dir(folder) do
        local filePath = folder .. "/" .. entry
        if string.match(string.lower(filePath), pattern) then
            ---@class state
            local state = {
                id = nil,
                path = filePath,
                name = entry,
                modification = lfs.attributes(filePath, "modification"),
                sha1 = nil,
                relative_path = string.sub(filePath, start_index, #filePath)
            }
            states[state.relative_path] = state
        end
    end
    return states
end

KoreanToChinese = {}
function KoreanToChinese.csvToMapTable(csv)
    local map = {}
    local rowNum = csv:getRowNumber()
    for i = 2, rowNum, 1 do
        local index = csv:getDataByHead(i, "index")
        if map[index] then
            error("repeated" .. index)
        else
            map[index] = {
                row = i,
                text = csv:getDataByHead(i, "text")
            }
        end
    end
    return map
end

function KoreanToChinese.collecCSD(folder)
    local fileNum = 0
    local all_csd_path = {}
    for entry in lfs.dir(folder) do
        if entry ~= "." and entry ~= ".." then
            local filePath = folder .. "\\" .. entry
            local fileAttributes = lfs.attributes(filePath)
            if fileAttributes.mode == "file" then
                if string.match(string.lower(filePath), "^.+%.csd$") then
                    all_csd_path[entry] = filePath
                    fileNum = fileNum + 1
                end
            end
        end
    end
    return all_csd_path, fileNum
end

---@param node XMLNode
---@param toCsv CSV
function KoreanToChinese.extractText(csdName, node, toCsv)
    local attributes = { "ButtonText", "LabelText", "PlaceHolderText" }
    for i, attri in ipairs(attributes) do
        if node:getAttributeValue(attri) then
            local row = toCsv:getRowNumber() + 1
            local tag = node:getAttributeValue("Tag")
            local name = node:getAttributeValue("Name")
            toCsv:setCellByHead(row, "text", node:getAttributeValue(attri))
            toCsv:setCellByHead(row, "csd", csdName)
            toCsv:setCellByHead(row, "name", node:getAttributeValue("Name"))
            toCsv:setCellByHead(row, "tag", tag)
            toCsv:setCellByHead(row, "attri", attri)
            toCsv:setCellByHead(row, "index", string.format("%s_%s_%s_%s", csdName, tag, name, attri))
            toCsv:setCellByHead(row, "replaced", "false")
        end
    end
    for k, v in pairs(node:getChildren()) do
        KoreanToChinese.extractText(csdName, v, toCsv)
    end
end

function KoreanToChinese.csdToCsv(csd_path, csv, fileNum)
    local i = 1
    local lastLen = 0
    for fileName, filePath in pairs(csd_path) do
        local csd = XML(filePath):getRootNode()
        local out = string.format("\r%s/%s %s", i, fileNum, fileName)
        local curLen = #out
        if lastLen > curLen then
            out = out .. string.rep(" ", lastLen - curLen)
        end
        lastLen = curLen
        io.write(out)
        i = i + 1
        KoreanToChinese.extractText(fileName, csd, csv)
    end
    print()
end

--- func desc
---@param csdName any
---@param node XMLNode
---@param fromMap any
function KoreanToChinese.replaceText(csdName, node, fromMap)
    local attributes = { "ButtonText", "LabelText", "PlaceHolderText" }
    for i, attri in ipairs(attributes) do
        if node:getAttributeValue(attri) then
            local tag = node:getAttributeValue("Tag")
            local name = node:getAttributeValue("Name")
            local value = fromMap[string.format("%s_%s_%s_%s", csdName, tag, name, attri)]
            if value then
                node:setAttributeValue(attri, value.text)
            end
        end
    end
    for k, v in pairs(node:getChildren()) do
        KoreanToChinese.replaceText(csdName, v, fromMap)
    end
end

function KoreanToChinese.CsvToCsd(csd_path, csv, fileNum)
    local i = 1
    local lastLen = 0
    for fileName, filePath in pairs(csd_path) do
        local csd = XML(filePath)
        local out = string.format("\r%s/%s %s", i, fileNum, fileName)
        local curLen = #out
        if lastLen > curLen then
            out = out .. string.rep(" ", lastLen - curLen)
        end
        lastLen = curLen
        io.write(out)
        i = i + 1
        KoreanToChinese.replaceText(fileName, csd:getRootNode(), csv)
        csd:writeTo(filePath)
    end
    print()
end

Win32 = {}
function Win32.getSha1(path)
    local windows_path = string.gsub(path, "/", "\\")
    local cmd = string.format('certutil -hashfile "%s"', windows_path)
    local result = io.popen(cmd) or error("can't run " .. cmd)
    local _, r = result:read("l"), result:read("l")
    result:close()
    return r
end

function Win32.copy(from, to)
    local windows_from = string.gsub(from, "/", "\\")
    local windows_to = string.gsub(to, "/", "\\")
    local cmd = string.format('copy "%s" "%s" /Y', windows_from, windows_to)
    local r = io.popen(cmd)
    if not r then
        error("can't run " .. cmd)
    else
        print(r:read("a"))
        r:close()
    end
end
