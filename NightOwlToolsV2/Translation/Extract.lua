for i, project in ipairs(Translation.Projects) do
    local ui_dir = project.source .. "/cocosstudio/ui"
    local ui_files = Common.GetFilesOfCurDir(ui_dir, "csd")
    local tableHead = {"text", "translation", "csd", "name", "tag", "attri", "index", "replaced"}
    local text = CSV()
    text:setTableHead(tableHead)
    Translation:csdToCsv(ui_files, text)
    local textCsv = project.source .. "/text.csv"
    local textXlsx = project.source .. "/text.xlsx"
    text:writeTo(textCsv, true)
    Translation:covert(textCsv, textXlsx)
end

