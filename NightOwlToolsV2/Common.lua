require "Tools.XML"
require "Tools.CSV"

Common = {}
function Common.ShowOnOneline(out)
    local out = "\r" .. out
    local curLen = #out
    out = out .. string.rep(" ", 100 - curLen)
    io.write(out)
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
    RootFolders = { --
    {
        from = "D:/Closers.cocos/resource/ui/branches/dzogame_sea_v1/zhcn",
        to = "D:/Closers.cocos/client/branches/dzogame_sea_v1/Resources/res_zhcn"
    }, --
    {
        from = "D:/Closers.cocos/resource/ui/branches/dzogame_sea/zhcn",
        to = "D:/Closers.cocos/client/branches/dzogame_sea/Resources/res_zhcn"
    }, --
    {
        from = "D:/Closers.cocos/resource/ui/branches/online",
        to = "D:/Closers.cocos/client/branches/online/Resources/res"
    } --
    }
}
function PublishResource.TouchTable(db, tableName)
    return db:exec(string.format([[
        CREATE TABLE %s (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            modification TIMESTAMP,
            sha1 TEXT,
            name TEXT,
            relative_path TEXT
        );
]], tableName))
end
function PublishResource.CollectFiles(folder, suffix)
    local allFiles = {}
    local pattern = "^.+%" .. suffix .. "$"
    for entry in lfs.dir(folder) do
        local filePath = folder .. "/" .. entry
        local fileAttributes = lfs.attributes(filePath)
        if string.match(string.lower(filePath), pattern) then
            Common.ShowOnOneline(entry)
            allFiles[#allFiles + 1] = {
                path = filePath,
                name = entry,
                modification = lfs.attributes(filePath, "modification")
            }
        end
    end
    Common.ShowOnOneline("CollectFiles Done : " .. folder)
    print()
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
