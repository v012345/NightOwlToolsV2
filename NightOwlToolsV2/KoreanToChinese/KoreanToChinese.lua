
---@type CSV
local tChineseText = CSV("LocalOnly/ChineseText.csv", true)
---@type CSV
local tKoreanText = CSV("LocalOnly/KoreanText.csv", true)

local ChineseMap = KoreanToChinese.csvToMapTable(tChineseText)
local KoreanMap = KoreanToChinese.csvToMapTable(tKoreanText)

for k, v in pairs(KoreanMap) do
    if ChineseMap[k] then
        tKoreanText:setCellByHead(v.row, "text", ChineseMap[k].text)
        tKoreanText:setCellByHead(v.row, "replaced", "true")
    end
end
tKoreanText:writeTo("LocalOnly/ReplacedText.csv", true)


