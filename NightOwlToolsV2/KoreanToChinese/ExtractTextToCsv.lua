local base = "D:/Closers.cocos/resource/ui/branches/online/cocosstudio/ui/"
local Korean = "D:/Closers.cocos/resource/ui/branches/qooapp/zhtw/cocosstudio/ui"
local tableHead = {"text", "csd", "name", "tag", "attri", "index", "replaced"}

local base_csd_path, baseFileNum = KoreanToChinese.collecCSD(base)
local Korean_csd_path, KoreanFileNum = KoreanToChinese.collecCSD(Korean)

local baseText = CSV()
baseText:setTableHead(tableHead)
local tKoreanText = CSV()
tKoreanText:setTableHead(tableHead)

KoreanToChinese.csdToCsv(base_csd_path, baseText, baseFileNum)
KoreanToChinese.csdToCsv(Korean_csd_path, tKoreanText, KoreanFileNum)
baseText:writeTo("LocalOnly/ChineseText.csv", true)
tKoreanText:writeTo("LocalOnly/KoreanText.csv", true)

