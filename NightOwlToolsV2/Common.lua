require "Tools.XML"
require "Tools.CSV"
local socket = require "socket.core"
local sha = require "pure_lua_SHA.sha2"

Common = Common or {}

function Common.sleep(s)
    socket.sleep(s)
end

function Common.Write(out)
    io.write("\r" .. out .. string.rep(" ", Common.GetConsoleScreenBufferInfo() - #tostring(out)))
end

function Common.Merge(...)
    local t = {}
    for _, arr in ipairs({...}) do
        for _, v in ipairs(arr) do
            t[#t + 1] = v
        end
    end
    return t
end

function Common.Checksum(file_path)
    local append = sha.md5() -- create calculation instance #1
    local file = io.open(file_path, "rb") or error()
    for chunk in file:lines(4096) do
        append(chunk)
    end
    file:close()
    return tostring(append())
end

function Common.Split(inputstr, sep)
    if sep == nil then
        sep = "%s"
    end
    local t = {}
    for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
        table.insert(t, str)
    end
    return t
end

---Get Files Of Current Directory With The Given Suffix
function Common.GetFilesOfCurDir(dir, suffix)
    suffix = string.lower(suffix)
    local pattern = "^.+%." .. suffix .. "$"
    local files = {}
    for entry in lfs.dir(dir) do
        if string.match(string.lower(entry), pattern) then
            files[#files + 1] = dir .. "/" .. entry
        end
    end
    return files
end

KoreanToChinese = {}
function KoreanToChinese.csvToMapTable(csv)
    local map = {}
    local rowNum = csv:getRowNumber()
    for i = 2, rowNum, 1 do
        local index = csv:getDataByHead(i, "index")
        if map[index] then
            error("repeated" .. index)
        else
            map[index] = {
                row = i,
                text = csv:getDataByHead(i, "text")
            }
        end
    end
    return map
end

function KoreanToChinese.collecCSD(folder)
    local fileNum = 0
    local all_csd_path = {}
    for entry in lfs.dir(folder) do
        if entry ~= "." and entry ~= ".." then
            local filePath = folder .. "\\" .. entry
            local fileAttributes = lfs.attributes(filePath)
            if fileAttributes.mode == "file" then
                if string.match(string.lower(filePath), "^.+%.csd$") then
                    all_csd_path[entry] = filePath
                    fileNum = fileNum + 1
                end
            end
        end
    end
    return all_csd_path, fileNum
end

---@param node XMLNode
---@param toCsv CSV
function KoreanToChinese.extractText(csdName, node, toCsv)
    local attributes = {"ButtonText", "LabelText", "PlaceHolderText"}
    for i, attri in ipairs(attributes) do
        if node:getAttributeValue(attri) then
            local row = toCsv:getRowNumber() + 1
            local tag = node:getAttributeValue("Tag")
            local name = node:getAttributeValue("Name")
            toCsv:setCellByHead(row, "text", node:getAttributeValue(attri))
            toCsv:setCellByHead(row, "csd", csdName)
            toCsv:setCellByHead(row, "name", node:getAttributeValue("Name"))
            toCsv:setCellByHead(row, "tag", tag)
            toCsv:setCellByHead(row, "attri", attri)
            toCsv:setCellByHead(row, "index", string.format("%s_%s_%s_%s", csdName, tag, name, attri))
            toCsv:setCellByHead(row, "replaced", "false")
        end
    end
    for k, v in pairs(node:getChildren()) do
        KoreanToChinese.extractText(csdName, v, toCsv)
    end
end

function KoreanToChinese.csdToCsv(csd_path, csv, fileNum)
    local i = 1
    local lastLen = 0
    for fileName, filePath in pairs(csd_path) do
        local csd = XML(filePath):getRootNode()
        local out = string.format("\r%s/%s %s", i, fileNum, fileName)
        local curLen = #out
        if lastLen > curLen then
            out = out .. string.rep(" ", lastLen - curLen)
        end
        lastLen = curLen
        io.write(out)
        i = i + 1
        KoreanToChinese.extractText(fileName, csd, csv)
    end
    print()
end

--- func desc
---@param csdName any
---@param node XMLNode
---@param fromMap any
function KoreanToChinese.replaceText(csdName, node, fromMap)
    local attributes = {"ButtonText", "LabelText", "PlaceHolderText"}
    for i, attri in ipairs(attributes) do
        if node:getAttributeValue(attri) then
            local tag = node:getAttributeValue("Tag")
            local name = node:getAttributeValue("Name")
            local value = fromMap[string.format("%s_%s_%s_%s", csdName, tag, name, attri)]
            if value then
                node:setAttributeValue(attri, value.text)
            end
        end
    end
    for k, v in pairs(node:getChildren()) do
        KoreanToChinese.replaceText(csdName, v, fromMap)
    end
end

function KoreanToChinese.CsvToCsd(csd_path, csv, fileNum)
    local i = 1
    local lastLen = 0
    for fileName, filePath in pairs(csd_path) do
        local csd = XML(filePath)
        local out = string.format("\r%s/%s %s", i, fileNum, fileName)
        local curLen = #out
        if lastLen > curLen then
            out = out .. string.rep(" ", lastLen - curLen)
        end
        lastLen = curLen
        io.write(out)
        i = i + 1
        KoreanToChinese.replaceText(fileName, csd:getRootNode(), csv)
        csd:writeTo(filePath)
    end
    print()
end

CopyDirStructure = {}
function CopyDirStructure.copyDirStructure(from, to, exclude)
    for entry in lfs.dir(from) do
        if entry ~= "." and entry ~= ".." then
            local path = from .. "/" .. entry
            local attri = lfs.attributes(path)
            if attri.mode == "directory" and not exclude[entry] then
                local target = to .. "/" .. entry
                if not lfs.attributes(target) then
                    lfs.mkdir(target)
                    print(target)
                end
                CopyDirStructure.copyDirStructure(path, target, exclude)
            end
        end
    end
end

Win32 = {}
function Win32.getSha1(path)
    local windows_path = string.gsub(path, "/", "\\")
    local cmd = string.format('certutil -hashfile "%s"', windows_path)
    local result = io.popen(cmd) or error("can't run " .. cmd)
    local _, r = result:read("l"), result:read("l")
    result:close()
    return r
end

function Win32.copy(from, to)
    local windows_from = string.gsub(from, "/", "\\")
    local windows_to = string.gsub(to, "/", "\\")
    local cmd = string.format('copy "%s" "%s" /Y', windows_from, windows_to)
    local r = io.popen(cmd)
    if not r then
        error("can't run " .. cmd)
    else
        print(r:read("a"))
        r:close()
    end
end

Socket = {}

function Socket.put_utf8_string(client, str)
    local len = #str + 1
    local b1 = (len & 0x000000ff) >> 0
    local b2 = (len & 0x0000ff00) >> 8
    local b3 = (len & 0x00ff0000) >> 16
    local b4 = (len & 0xff000000) >> 24
    return client:send(string.char(b1, b2, b3, b4) .. str)

end
function Socket.get_utf8_string(client)
    local data, err = client:receive()
    if err then
        return data, err
    end
    local b4 = string.byte(data, 4) << 24
    local b3 = string.byte(data, 3) << 16
    local b2 = string.byte(data, 2) << 8
    local b1 = string.byte(data, 1)
    local len = b1 + b2 + b3 + b4
    local tLen = #data
    if tLen ~= len + 3 then
        error("这个情况我还不太处理, 大概要分包了")
    end
    local rec = string.sub(data, 5, tLen)
    return rec, err
end
