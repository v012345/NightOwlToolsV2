-- local base = "D:/Closers.cocos/resource/ui/branches/handygame_test/ko/cocosstudio/ui/"
-- local tableHead = {"text", "csd", "name", "tag", "attri", "index", "replaced"}
-- local base_csd_path, baseFileNum = KoreanToChinese.collecCSD(base)
-- local baseText = CSV("D:/NightOwlToolsV2/NightOwlToolsV2/LocalOnly/Copy of need_trans.csv",true)
-- baseText:setTableHead(tableHead)
-- KoreanToChinese.csdToCsv(base_csd_path, baseText, baseFileNum)
-- baseText:writeTo("LocalOnly/base.csv", true)
-- local tReplaceText = CSV("LocalOnly/ReplacedText.csv", true)
-- local replaceMap = KoreanToChinese.csvToMapTable(baseText)
-- local Korean = "D:/Closers.cocos/resource/ui/branches/handygame_test/ko/cocosstudio/ui/"
-- local Korean_csd_path, KoreanFileNum = KoreanToChinese.collecCSD(Korean)
-- KoreanToChinese.CsvToCsd(Korean_csd_path, replaceMap, KoreanFileNum)
for i, project in ipairs(Translation.Projects) do
    local textCsv = project.source .. "/text.csv"
    local textXlsx = project.source .. "/text.xlsx"
    Translation:covert(textXlsx, textCsv)

    local ui_dir = project.source .. "/cocosstudio/ui"
    local ui_files = Common.GetFilesOfCurDir(ui_dir, "csd")
    -- local tableHead = {"text", "translation", "csd", "name", "tag", "attri", "index", "replaced"}
    local text = CSV(textCsv, true)
    -- Translation:inje(ui_files, text)

    text:writeTo(textCsv, true)
end

