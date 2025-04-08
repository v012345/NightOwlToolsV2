-- local utf8 = require("utf8")
---@type CSV
local needTransCsv = CSV("LocalOnly/needTransCsv.csv", true)
---@type CSV
local text = CSV("LocalOnly/text.csv", true)
needTransCsv:setIndex("index")
text:setIndex("index")
for i = 2, needTransCsv:getRowNumber(), 1 do
    local index = needTransCsv:getDataByHead(i,"index")
    local data =  text:getDataFromIndex("index",index,"text")
    if data then
        needTransCsv:setCellByHead(i,"text",data)
    end
end
needTransCsv:writeTo("LocalOnly/needTransCsv1.csv")
Translation:covert("LocalOnly/needTransCsv1.csv", "LocalOnly/needTransCsv1.xlsx")
-- ---@type CSV
-- local filter = CSV()
-- filter:setTableHead(tReplaceText:getHead())
-- local allRow = tReplaceText:getRowNumber()
-- for i = 2, allRow, 1 do
--     local text = tReplaceText:getDataByHead(i, "text")
--     local foundKorean = false
--     for _, codepoint in utf8.codes(text) do
--         if codepoint >= 0x4E00 and codepoint <= 0x9FA5 then
--             foundKorean = true
--             break
--         end
--     end
--     if foundKorean then
--         filter:copyFromRow(filter:getRowNumber() + 1, tReplaceText:getRow(i))
--     end
-- end
-- filter:writeTo("LocalOnly/need_trans.csv", true)
-- local Korean = "D:/Closers.cocos/resource/ui/branches/qooapp/zhtw/cocosstudio/ui"
-- local Korean_csd_path, KoreanFileNum = KoreanToChinese.collecCSD(Korean)
-- KoreanToChinese.CsvToCsd(Korean_csd_path, replaceMap, KoreanFileNum)
