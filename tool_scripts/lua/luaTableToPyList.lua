require "lua.Common"
local specail = {
    ["temp.csv_merge.cfg_CuiPanGuan"] = "idx",
    ["temp.csv_merge.cfg_HuiShouCategories"] = "CategoryID",
    ["temp.csv_merge.cfg_HuiShouSubcategories"] = "SubcategoryID",
    ["temp.csv_merge.cfg_HuoDongDaTing"] = "actIndex",
    ["temp.csv_merge.cfg_JuQing"] = "idx",
    ["temp.csv_merge.cfg_JuQingCategories"] = "CategoryID",
    ["temp.csv_merge.cfg_ZhuXianRenWuCategories"] = "CategoryID"
}

local function cell_to_string(t)
    if type(t) ~= "table" then
        return tostring(t)
    end
    local s = ""
    for i, v in ipairs(t) do
        if type(v) == "table" then
            for i1, v1 in ipairs(v) do
                if type(v1) == "table" then
                    error("不支持这么多维的表")
                else
                    s = s .. v1
                    if i1 < #v then
                        s = s .. "^"
                    end
                end
            end
            if i < #t then
                s = s .. "|"
            end
        else
            s = s .. v
            if i < #t then
                s = s .. "|"
            end
        end
    end
    return s
end

local function dump_to_py(lua_table, key, table_head, to_where, name)
    local file = io.open(to_where, "w")
    file:write(string.format("%s = [\n", name))
    local has_key = table_head[key]

    if has_key then
        table_head[key] = nil
    end
    local new_head = {}
    for k, v in pairs(table_head) do
        new_head[#new_head + 1] = k
    end
    file:write("    [")
    file:write(string.format('"%s",', key))
    for i, v in ipairs(new_head) do
        file:write(string.format('"%s",', v))
    end

    file:write("],\n")

    for k, v in pairs(lua_table) do
        file:write("    [")
        if has_key then
            file:write(string.format('"%s",', v[key]))
        else
            file:write(string.format('"%s",', k))
        end
        for i, head in ipairs(new_head) do
            if v[head] then
                file:write(string.format('"%s",', string.gsub(cell_to_string(v[head]), "\n", "\\n")))
            else
                file:write('"",')
            end
        end
        file:write("],\n")
    end

    file:write("]")
    file:close()
end

local lua_table = require(arg[1])
local to_where = arg[2]
local name = arg[3]
local header = {}
for _, t in pairs(lua_table) do
    for k, v in pairs(t) do
        header[k] = true
    end
end
local key = specail[arg[1]] or "xls_id"
dump_to_py(lua_table, key, header, to_where, name)

