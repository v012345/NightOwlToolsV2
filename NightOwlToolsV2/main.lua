require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "KoreanToChinese.KoreanToChinese"
end, function(msg)
    print(msg)
end)
