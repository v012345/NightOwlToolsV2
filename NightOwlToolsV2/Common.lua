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
    PublishAll = false, -- 全部发布
    Projects = { --
    {
        from = "D:/Closers.cocos/resource/ui/branches/dzogame_sea_v1/zhcn",
        to = "D:/Closers.cocos/client/branches/dzogame_sea_v1/Resources/res_zhcn"
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
function PublishResource.PublishPlist(states_will_publish, css_file_place_path, publish_directory,
    cocosstudio_directory_path)
    local num = Common.GetMapItemNum(states_will_publish)
    if num <= 0 then
        return
    end
    ---@type XML
    local css_file_template = XML(PublishResource.CCS_Template)

    ---@type XMLNode
    local RootFolder_node = css_file_template:getRootNode():getChild(2):getChild(1):getChild(1)
    for i, v in pairs(states_will_publish) do
        print(v.name)
        local ImageFiles_node = XML(v.path):getRootNode():getChild(2):getChild(1):getChildren()
        for i, v in ipairs(ImageFiles_node) do
            local rp = v:getAttributeValue("Path")
            local node_temp = RootFolder_node
            local temp_cocosstudio_directory_path = cocosstudio_directory_path
            for k, v in pairs(Common.Split(rp, "/")) do
                temp_cocosstudio_directory_path = temp_cocosstudio_directory_path .. "/" .. v
                local fileAttributes = lfs.attributes(temp_cocosstudio_directory_path)
                if fileAttributes.mode == "directory" then
                    local sub_node = node_temp:getChildByAttri("Name", v)
                    if not sub_node then
                        sub_node = XML:newNode("Folder")
                        sub_node:setAttributeValue("Name", v)
                        node_temp:addChild(sub_node)
                    end
                    node_temp = sub_node
                elseif fileAttributes.mode == "file" then
                    local sub_node = node_temp:getChildByAttri("Name", v)
                    if not sub_node then
                        sub_node = XML:newNode("Image")
                        sub_node:setAttributeValue("Name", v)
                        node_temp:addChild(sub_node)
                    end
                end
            end
        end
    end
    ---@type XMLNode
    local ui_node = css_file_template:getRootNode():getChild(2):getChild(1):getChild(1):getChildByAttri("Name", "plist")
    for i, v in pairs(states_will_publish) do
        ---@type XMLNode
        local newNode = XML:newNode("PlistInfo")
        newNode:setAttributeValue("Name", v.name)
        newNode:setAttributeValue("Type", "Plist")
        ui_node:addChild(newNode)
    end
    local temp_css_file = css_file_place_path .. "/temp_css_file.ccs"
    css_file_template:writeTo(temp_css_file)
    Common.ShowOnOneline("start publish, please wait")
    local cocos_cmd = '"%s" publish -f %s -o %s -s -d Serializer_FlatBuffers'
    local cmd = string.format(cocos_cmd, PublishResource.CocosTool, temp_css_file, publish_directory)
    local exe_cmd = io.popen(cmd) or error("can't execute " .. cocos_cmd)
    local result = exe_cmd:read("a")
    exe_cmd:close()
    os.remove(temp_css_file)
    if string.find(result, "Publish success!") then
        Common.ShowOnOneline("Publish success!")
        print()
    else
        error(result)
    end
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

---@return state
function PublishResource.GetFileNowState(name, relative_path, path)
    return {
        id = nil,
        path = path,
        name = name,
        modification = lfs.attributes(path, "modification"),
        sha1 = nil,
        relative_path = relative_path
    }
end

function PublishResource.GetChangedImageOfPlist(db, plist_state, image_table_name, root_folder)
    if string.sub(root_folder, #root_folder, #root_folder) ~= "/" then
        root_folder = root_folder .. "/"
    end
    local images_current_states = {}
    ---@type XMLNode[]
    local ImageFiles = XML(plist_state.path):getRootNode():getChild(2):getChild(1):getChildren()
    for i, v in ipairs(ImageFiles) do
        local rp = v:getAttributeValue("Path")
        images_current_states[rp] = PublishResource.GetFileNowState(rp, rp, root_folder .. rp)
    end
    PublishResource.TouchTable(db, image_table_name)
    local image_last_state = PublishResource.GetLastStates(db, image_table_name)
    local to_publish_image = PublishResource.GetPublishState(images_current_states, image_last_state)
    return to_publish_image
end

---@param now state[]
---@param last state[]
---@return state[],state[]
function PublishResource.GetPublishState(now, last)
    local need_publish_state = {}
    local unneed_publish_state = {}
    for k, v in pairs(now) do
        local old = last[k]
        if old then
            v.id = old.id
            if old.modification < v.modification and Win32.getSha1(v.path) ~= old.sha1 then
                need_publish_state[k] = v
            else
                unneed_publish_state[k] = v
            end
        else
            need_publish_state[k] = v
        end
    end
    return need_publish_state, unneed_publish_state
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
function PublishResource.GetFilesCurrentState(folder, suffix, root_folder)
    if string.sub(root_folder, #root_folder, #root_folder) ~= "/" then
        root_folder = root_folder .. "/"
    end
    local cut_start_index = #root_folder + 1
    ---@type state[]
    local allFiles = {}
    local pattern = "^.+%" .. suffix .. "$"
    for entry in lfs.dir(folder) do
        local filePath = folder .. "/" .. entry
        local fileAttributes = lfs.attributes(filePath)
        if string.match(string.lower(filePath), pattern) then
            ---@class state
            local state = {
                id = nil,
                path = filePath,
                name = entry,
                modification = lfs.attributes(filePath, "modification"),
                sha1 = nil,
                relative_path = string.sub(filePath, cut_start_index, #filePath)
            }
            allFiles[state.relative_path] = state
        end
    end
    return allFiles
end

function PublishResource.CollectImageRess(folder)
    local allFiles = {}
    local function TravelFiles(folder)
        for entry in lfs.dir(folder) do
            if entry ~= "." and entry ~= ".." then
                local filePath = folder .. "/" .. entry
                local fileAttributes = lfs.attributes(filePath)
                if fileAttributes.mode == "directory" then
                    TravelFiles(filePath)
                elseif fileAttributes.mode == "file" then
                    Common.ShowOnOneline(entry)
                    allFiles[#allFiles + 1] = {
                        path = filePath,
                        name = entry,
                        modification = lfs.attributes(filePath, "modification")
                    }
                end
            end
        end
    end
    TravelFiles(folder)
    return allFiles
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
    local attributes = {"ButtonText", "LabelText", "PlaceHolderText"}
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
    local attributes = {"ButtonText", "LabelText", "PlaceHolderText"}
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
