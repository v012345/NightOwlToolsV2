require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Common"
    -- require "KoreanToChinese.ExtractTextToCsv"
    -- require "KoreanToChinese.KoreanToChinese"
    -- require "KoreanToChinese.ReplaceTextToCsd"
    -- require "KoreanToChinese.FilterReplaceText"
    require "PublishResource.PublishCsd"
    require "PublishResource.PublishPlist"
end, function(msg)
    print(msg)
end)
