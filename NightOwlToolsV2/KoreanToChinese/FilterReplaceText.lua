local utf8 = require("utf8")
---@type CSV
local tReplaceText = CSV("LocalOnly/ReplacedText.csv", true)
local replaceMap = KoreanToChinese.csvToMapTable(tReplaceText)
---@type CSV
local filter = CSV()
filter:setTableHead(tReplaceText:getHead())
local allRow = tReplaceText:getRowNumber()
for i = 2, allRow, 1 do
    local isReplaced = tReplaceText:getDataByHead(i, "replaced")
    if isReplaced == "false" then
        local text = tReplaceText:getDataByHead(i, "text")
        local foundKorean = false
        for _, codepoint in utf8.codes(text) do
            if codepoint >= 0xAC00 and codepoint <= 0xD7A3 then
                foundKorean = true
                break
            end
        end
        if foundKorean then
            filter:copyFromRow(filter:getRowNumber() + 1, tReplaceText:getRow(i))
        end
    end
end
filter:writeTo("LocalOnly/filterText.csv", true)
-- local Korean = "D:/Closers.cocos/resource/ui/branches/qooapp/zhtw/cocosstudio/ui"
-- local Korean_csd_path, KoreanFileNum = KoreanToChinese.collecCSD(Korean)
-- KoreanToChinese.CsvToCsd(Korean_csd_path, replaceMap, KoreanFileNum)
