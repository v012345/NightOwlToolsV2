require "Translation.config"
function Translation:init()
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
    self.ConvertTool = string.format("%s\\LocalOnly\\TableConvertor\\TableConvertor.exe", lfs.currentdir())
end

---@param node XMLNode
---@param toCsv CSV
function Translation:extractText(csdName, node, toCsv)
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
            toCsv:setCellByHead(row, "translation", "")
        end
    end
    for k, v in pairs(node:getChildren()) do
        self:extractText(csdName, v, toCsv)
    end
end

function Translation:covert(from, to)
    local cmd = string.format('cmd /c ""%s" "%s" "%s""', self.ConvertTool, from, to) -- 不知道为什么会这样?
    os.execute(cmd)
end

---@param node XMLNode
---@param fromCsv CSV
function Translation:injectText(csdName, node, fromCsv,needTransCsv)
    local attributes = {"ButtonText", "LabelText", "PlaceHolderText"}
    for i, attri in ipairs(attributes) do
        if node:getAttributeValue(attri) then

            local tag = node:getAttributeValue("Tag")
            local name = node:getAttributeValue("Name")
            local index = string.format("%s_%s_%s_%s", csdName, tag, name, attri)
            local text = fromCsv:getDataFromIndex("index",index,"text")
            if text then
                node:setAttributeValue(attri,text)
            else
                local row = needTransCsv:getRowNumber() + 1
                needTransCsv:setCellByHead(row, "text", node:getAttributeValue(attri))
                needTransCsv:setCellByHead(row, "csd", csdName)
                needTransCsv:setCellByHead(row, "name", node:getAttributeValue("Name"))
                needTransCsv:setCellByHead(row, "tag", tag)
                needTransCsv:setCellByHead(row, "attri", attri)
                needTransCsv:setCellByHead(row, "index", string.format("%s_%s_%s_%s", csdName, tag, name, attri))
                needTransCsv:setCellByHead(row, "replaced", "false")
                needTransCsv:setCellByHead(row, "translation", "")
            end
        end
    end
    for k, v in pairs(node:getChildren()) do
        self:injectText(csdName, v, fromCsv,needTransCsv)
    end
end

function Translation:inject(csd_path, text)
    local i = 1
    local total = #csd_path
    text:setIndex("index")
    local tableHead = {"text", "translation", "csd", "name", "tag", "attri", "index", "replaced"}
    local needTransCsv = CSV()
    needTransCsv:setTableHead(tableHead)
    for _, filePath in pairs(csd_path) do
        local t = Common.Split(filePath, "/")
        local fileName = t[#t]
        local xml = XML(filePath)
        Common.Write(string.format("\r%s/%s %s", i, total, fileName))
        i = i + 1
        self:injectText(fileName, xml:getRootNode(), text,needTransCsv)
        xml:writeTo(filePath)
    end
    needTransCsv:writeTo("LocalOnly/needTransCsv.csv")
    print()
end

function Translation:csdToCsv(csd_path, csv)
    local i = 1
    local lastLen = 0
    local total = #csd_path
    for _, filePath in pairs(csd_path) do
        local t = Common.Split(filePath, "/")
        local fileName = t[#t]
        local csd = XML(filePath):getRootNode()
        Common.Write(string.format("\r%s/%s %s", i, total, fileName))
        i = i + 1
        self:extractText(fileName, csd, csv)
    end
    print()
end

Translation:init()

return Translation
