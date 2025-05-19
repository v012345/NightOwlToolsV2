require "lua.Common"

local server = arg[1]
local client = arg[2]
local head = arg[3]
local name = arg[4]
local merged = arg[5]

local function deal_table(key, ki, ik, sTable, cTable,sort_by)

    local keyIndex = ki[key]
    local sTableReindex = {}
    if keyIndex then
        for i, v in ipairs(sTable.data) do
            sTableReindex[v[keyIndex]] = v
        end
    else
        sTableReindex = sTable.data
    end
    local mergedTable = {}
    for k, cv in pairs(cTable) do
        if sTableReindex[k] then
            local _t = sTableReindex[k]

            for k, v in pairs(cv) do
                if not ki[k] then
                    print(k)
                end
                _t[ki[k]] = v
            end
            mergedTable[#mergedTable + 1] = _t
        else
            local _t = {}
            for i, v in pairs(ik) do
                _t[i] = ""
            end
            for k, v in pairs(cv) do
                _t[ki[k]] = v
            end
            mergedTable[#mergedTable + 1] = _t
        end
    end
    table.sort(mergedTable, function(a, b)
        return a[ki[sort_by]] < b[ki[sort_by]]
    end)
    return mergedTable
end

local function dump_to_py(dealed_table, to_where, t, comment, table_head)
    local file = io.open(to_where, "w")
    file:write(string.format("%s = [\n", t))
    for i, v in ipairs(comment) do
        file:write("    [")
        for i, cell in ipairs(v) do
            if type(cell) == "string" then
                local temp_string = string.gsub(cell, "\n", "{lua_plz_new_line}")
                temp_string = string.format("%q, ", temp_string)
                file:write((string.gsub(temp_string, "{lua_plz_new_line}", "\\n")))
            elseif type(cell) == "number" then
                file:write(string.format('%s, ', cell))
            else
                error("未知类型")
            end
        end
        file:write("],\n")
    end
    file:write("    [")
    for i, v in ipairs(table_head) do
        if i == 1 then
            if type(v) == "string" then
                file:write(string.format("%q, ", "//" .. v))
            elseif type(v) == "number" then
                file:write(string.format('//%s, ', v))
            else
                error("未知类型")
            end
        else
            if type(v) == "string" then
                local temp_string = string.gsub(v, "\n", "{lua_plz_new_line}")
                temp_string = string.format("%q, ", temp_string)
                file:write((string.gsub(temp_string, "{lua_plz_new_line}", "\\n")))
            elseif type(v) == "number" then
                file:write(string.format('%s, ', v))
            else
                error("未知类型")
            end
        end
    end
    file:write("],\n")
    for i, v in ipairs(dealed_table) do
        file:write("    [")
        for i, cell in ipairs(v) do
            if type(cell) == "string" then
                local temp_string = string.gsub(cell, "\n", "{lua_plz_new_line}")
                temp_string = string.format("%q, ", temp_string)
                file:write((string.gsub(temp_string, "{lua_plz_new_line}", "\\n")))
            elseif type(cell) == "number" then
                file:write(string.format('%s, ', cell))
            else
                error("未知类型")
            end
        end
        file:write("],\n")
    end
    file:write("]")
    file:close()
end

local sTable = require(server)
local cTable = require(client)
local header = require(head)
local key = header.lua.key
local ki = header.lua.ki
local ik = {}
for k, v in pairs(ki) do
    ik[v] = k
end
local sort_by = header.lua.sort_by
local mergedTable = deal_table(key, ki, ik, sTable, cTable, sort_by)
dump_to_py(mergedTable, merged, name, sTable.comment, ik)
ik = {}

