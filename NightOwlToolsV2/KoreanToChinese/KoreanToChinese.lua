print("KoreanToChinese")
require "Tools.XML"
require "Tools.CSV"
require "Tools.Log"

local online = "D:/Closers.cocos/resource/ui/branches/online/cocosstudio/ui/"
local Korean = "D:/Closers.cocos/resource/ui/branches/handygame/ko/cocosstudio/ui/"

local function collecCSD(folder)
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

local Chinese_csd_path, ChineseFileNum = collecCSD(online)
local Korean_csd_path, KoreanFileNum = collecCSD(Korean)

---@type CSV
local tChineseText = CSV()
local tableHead = {"text", "csd", "name", "tag", "attri"}
for i, v in ipairs(tableHead) do
    tChineseText:setCell(1, i, v)
end

---@type CSV
local tKoreanText = CSV()
for i, v in ipairs(tableHead) do
    tKoreanText:setCell(1, i, v)
end

---@param node XMLNode
---@param toCsv CSV
local function extractText(csdName, node, toCsv)
    local attributes = {"ButtonText", "LabelText", "PlaceHolderText"}
    for _, attri in ipairs(attributes) do
        if node:getAttributeValue(attri) then
            local row = toCsv:getRowNumber() + 1
            toCsv:setCell(row, 1, node:getAttributeValue(attri))
            toCsv:setCell(row, 2, csdName)
            toCsv:setCell(row, 3, node:getAttributeValue("Name"))
            toCsv:setCell(row, 4, node:getAttributeValue("Tag"))
            toCsv:setCell(row, 5, attri)
        end
    end
    for k, v in pairs(node:getChildren()) do
        extractText(csdName, v, toCsv)
    end
end
local function csdToCsv(csd_path, csv, fileNum)
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
        extractText(fileName, csd, csv)
    end
    print()
end
csdToCsv(Chinese_csd_path, tChineseText, ChineseFileNum)
csdToCsv(Korean_csd_path, tKoreanText, KoreanFileNum)
tChineseText:writeTo("LocalOnly/ChineseText.csv", true)
tKoreanText:writeTo("LocalOnly/KoreanText.csv", true)

