local text_tables = {"text_ui", "text_name","text_content"}

for i, text_table in ipairs(text_tables) do

    local text_ui_en = dofile(string.format(
        "C:\\work\\Closers.cocos\\client\\branches\\juefeng_sea\\Resources\\src\\imports\\table\\%s_en.lua", text_table))
    local text_ui_zhcn = dofile(string.format(
        "C:\\work\\Closers.cocos\\client\\branches\\juefeng_sea\\Resources\\src\\imports\\table\\%s_zhcn.lua", text_table))
    local text_ui_en_1 = dofile(string.format(
        "C:\\work\\Closers.cocos\\client\\branches\\juefeng_sea\\Resources\\src\\imports\\table\\%s_en_1.lua", text_table))
    local oupt = CSV()
    local r = oupt:getRowNumber() + 1
    oupt:setCell(r, 1, "如果为 -1 说明是新增")
    oupt:setCell(r, 2, "原来的英文")
    oupt:setCell(r, 3, "优化的英文")
    oupt:setCell(r, 4, "对应的中文")
    oupt:setCell(r, 5, "如果新增请这使用这个 id")
    print(#text_ui_en_1)
    for i, v in pairs(text_ui_en) do
        if text_ui_en_1[i] then
            if text_ui_en_1[i].en ~= v.en then
                local r = oupt:getRowNumber() + 1
                oupt:setCell(r, 1, i)
                oupt:setCell(r, 2, v.en)
                oupt:setCell(r, 3, text_ui_en_1[i].en)
                oupt:setCell(r, 4, text_ui_zhcn[i].zhcn)
                oupt:setCell(r, 5, i)
            end
        else
            local r = oupt:getRowNumber() + 1
            oupt:setCell(r, 1, i)
            oupt:setCell(r, 2, v.en)
            oupt:setCell(r, 3, v.en)
            oupt:setCell(r, 4, text_ui_zhcn[i].zhcn)
            oupt:setCell(r, 5, i)
        end
    end
    for k, v in pairs(text_ui_en_1) do
        if not text_ui_en[k] then
            local r = oupt:getRowNumber() + 1
            oupt:setCell(r, 1, -1)
            oupt:setCell(r, 2, v.en)
            oupt:setCell(r, 3, v.en)
            oupt:setCell(r, 4, v.en)
            oupt:setCell(r, 5, k)
        end
    end
    oupt:writeTo("LocalOnly/"..text_table.."_diff.csv")
    
    Translation:covert("LocalOnly/"..text_table.."_diff.csv", "C:/Users/Meteor/Desktop/out/"..text_table.."_diff.xlsx")
    -- Translation:covert("LocalOnly/text_ui_diff.csv", "LocalOnly/text_ui_diff.xlsx")

end
