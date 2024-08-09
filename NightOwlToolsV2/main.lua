require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Common"
    PublishResource:init()
    -- require "KoreanToChinese.ExtractTextToCsv"
    -- require "KoreanToChinese.KoreanToChinese"
    -- require "KoreanToChinese.ReplaceTextToCsd"
    -- require "KoreanToChinese.FilterReplaceText"
    -- require "PublishResource.PublishCsd"
    -- require "PublishResource.PublishPlist"
    -- require "Translation.ExtractTextToCsv"
    -- require "Translation.FilterChinese"
    -- require "Translation.Inject"
    require "Translation.Extract"
    PublishResource:realse()

end, function(msg)
    print(msg)
end)


