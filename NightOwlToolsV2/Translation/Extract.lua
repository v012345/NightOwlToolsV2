local url = "https://github.com/v012345/NightOwlToolsV2CS/releases/download/Tool-TableConvertor/TableConvertor.zip"
local TableConvertor = lfs.attributes("LocalOnly/TableConvertor/TableConvertor.exe")
if not TableConvertor then
    local TableConvertorZip = lfs.attributes("LocalOnly/TableConvertor.zip")
    if not TableConvertorZip then
        print("Downloading TableConvertor")
        local handle = io.popen("curl -s -L -o LocalOnly/TableConvertor.zip " .. url) -- 添加 -L 以跟随重定向
        handle:close()
        local TableConvertorZip = lfs.attributes("LocalOnly/TableConvertor.zip")
        print("Downloaded file size: " .. TableConvertorZip.size)
    end
    os.execute("mkdir LocalOnly\\TableConvertor")
    os.execute("tar -xf LocalOnly/TableConvertor.zip -C LocalOnly/TableConvertor")
end

-- local base = "D:/Closers.cocos/resource/ui/branches/handygame_test/ko/cocosstudio/ui/"
-- local tableHead = {"text", "csd", "name", "tag", "attri", "index", "replaced"}
-- local base_csd_path, baseFileNum = KoreanToChinese.collecCSD(base)
-- local baseText = CSV()
-- baseText:setTableHead(tableHead)
-- KoreanToChinese.csdToCsv(base_csd_path, baseText, baseFileNum)
-- baseText:writeTo("LocalOnly/base.csv", true)
-- print(lfs.currentdir() .. "/LocalOnly/base.csv")
local a = lfs.currentdir() .. "\\LocalOnly\\base.csv"
local b = lfs.currentdir() .. "\\LocalOnly\\base.xlsx"

local xx = string.format('"%s\\LocalOnly\\TableConvertor\\TableConvertor.exe" "%s" "%s"', lfs.currentdir(), a, b)
local cmd = string.format('cmd /c "%s"', xx) -- 不知道为什么会这样?
os.execute(cmd)
print(cmd)
