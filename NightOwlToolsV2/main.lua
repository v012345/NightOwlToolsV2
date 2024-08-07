require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Common"
    PublishResource:init()
    -- require "KoreanToChinese.ExtractTextToCsv"
    -- require "KoreanToChinese.KoreanToChinese"
    -- require "KoreanToChinese.ReplaceTextToCsd"
    -- require "KoreanToChinese.FilterReplaceText"
    require "PublishResource.PublishCsd"
    -- require "PublishResource.PublishPlist"

    PublishResource:realse()

end, function(msg)
    print(msg)
end)
