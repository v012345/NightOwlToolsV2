---@type CSV
local tReplaceText = CSV("LocalOnly/ReplacedText.csv", true)
local replaceMap = KoreanToChinese.csvToMapTable(tReplaceText)
local Korean = "D:/Closers.cocos/resource/ui/branches/qooapp/zhtw/cocosstudio/ui"
local Korean_csd_path, KoreanFileNum = KoreanToChinese.collecCSD(Korean)
KoreanToChinese.CsvToCsd(Korean_csd_path, replaceMap, KoreanFileNum)
