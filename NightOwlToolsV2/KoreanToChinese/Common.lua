require "Tools.XML"
require "Tools.CSV"

---@param csv CSV
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
