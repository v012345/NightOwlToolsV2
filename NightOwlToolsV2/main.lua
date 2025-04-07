require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    for k, v in pairs(arg) do
        print(k, v)
    end
    print("jjjjjjj")
    -- require "Scripts.CopyAToB"
    -- require "BlackMoonTools.Hotfix"
    -- require "PublishRes.PublishRes"
    -- require "Translation.Translation"
    -- -- require "Translation.Inject"
    -- -- PublishRes:init()
    -- -- require "KoreanToChinese.ExtractTextToCsv"
    -- -- require "KoreanToChinese.KoreanToChinese"
    -- -- require "KoreanToChinese.ReplaceTextToCsd"
    -- -- require "KoreanToChinese.FilterReplaceText"
    -- require "PublishRes.PublishCsd"
    -- require "PublishRes.PublishPlist"
    -- require "Translation.ExtractTextToCsv"
    -- require "Translation.FilterChinese"
    -- require "Translation.Extract"
    -- require "Translation.Inject"
    -- require "Translation.Filter"
    -- require "BlackMoonTextDiff.Main"
    -- PublishRes:realse()
end, function(msg)
    print(msg)
end)
