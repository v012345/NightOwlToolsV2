require "Tools.Parser"
---@class CSV:Parser
---@field _mData CSVCell[][]
CSV = {
    __parent = Parser
}

setmetatable(CSV, {
    __call = function(self, pathOrStream, hasHead)
        ---@type CSV
        local obj = {}
        setmetatable(obj, {
            __index = self
        })
        obj:_construct(pathOrStream, hasHead)
        return obj
    end,
    __index = CSV.__parent
})

function CSV:_construct(pathOrStream, hasHead)
    CSV.__parent._construct(self, pathOrStream)
    self._mCsvRowNumber = 0
    self._mCsvColNumber = 0
    self._mData = {}
    self._maxColNumber = 0
    self._hasHead = hasHead and true or false
    self._tableHead = {}
    if pathOrStream then
        self:_parse()
    end
    if self._hasHead then
        local head = {}
        for i, v in ipairs(self._mData[1]) do
            head[i] = v:getData()
        end

        self:setTableHead(head)
    end
end

function CSV:getHead()
    local head = {}
    for k, v in pairs(self._tableHead) do
        head[v] = k
    end
    return head
end

function CSV:copyFromRow(rowNum, row)
    for i, v in ipairs(row) do
        self:setCell(rowNum, i, v:getData())
    end
end

function CSV:setTableHead(head)
    for i, v in ipairs(head) do
        if self._tableHead[v] then
            error("repeat head " .. v)
        else
            self._tableHead[v] = i
            self:setCell(1, i, v)
        end

    end
    self._hasHead = true
end

function CSV:setCellByHead(row, colName, data)
    self:setCell(row, self._tableHead[colName], data)
end

function CSV:setIndex(colName)
    if self._tableHead[colName] then
        self.__indexMap = self.__indexMap or {}
        self.__indexMap[colName] = self.__indexMap[colName] or {}
        local index = self.__indexMap[colName]
        local indexCol = self._tableHead[colName]
        for i = 2, #self._mData, 1 do
            local indexValue = self._mData[i][indexCol]:getData()
            if index[indexValue] then
                error("index repeat")
            else
                index[indexValue]= i
            end
             
        end
    end
end

function CSV:getDataFromIndex(index,indexValue,colName)
    if self.__indexMap[index] then
        if self.__indexMap[index][indexValue] then
            return self._mData[self.__indexMap[index][indexValue]][self._tableHead[colName]]:getData()
        else
            return nil
        end
    end
end

function CSV:getData(row, col)
    return self._mData[row][col]:getData()
end

function CSV:getDataByHead(row, colName)
    return self._mData[row][self._tableHead[colName]]:getData()
end

function CSV:_parse()
    repeat
        local row = {}
        while self._mCurrentChar ~= "\n" do
            row[#row + 1] = self:_readString()
        end
        self._mData[#self._mData + 1] = row
        self._maxColNumber = (#row < self._maxColNumber) and self._maxColNumber or #row
        self:_getNextChar()
    until self:_isEnd()
end

function CSV:writeTo(path, useBom)
    local file = io.open(path, "w") or error("can't open " .. path)
    if useBom then
        file:write("\xEF\xBB\xBF")
    end
    for _, row in ipairs(self._mData) do
        for i, cell in ipairs(row) do
            file:write(cell:getRawData())
            if i < #row then
                file:write(",")
            end
        end
        local supplementary = self._maxColNumber - #row
        if supplementary > 0 then
            file:write(string.rep(",", supplementary))
        end
        file:write("\n")
    end
    file:close()
end

---@param data string|nil
---@return CSVCell
function CSV:_spawnCell(data)
    ---@class CSVCell
    local cell = {
        getData = function(this)
            return this._mData
        end,
        setData = function(this, mData)
            if string.match(mData, '["|,]') then
                this._useQuotation = true
            end
            this._mData = mData
        end,
        isUseQuotation = function(this)
            return this._useQuotation
        end,
        getRawData = function(this)
            if this._useQuotation then
                return string.format('"%s"', string.gsub(this._mData, '"', '""'))
            else
                return this._mData
            end
        end,
        _useQuotation = false,
        _mData = ""
    }
    if data then
        cell:setData(data)
    end
    return cell
end

function CSV:getRow(rowNum)
    return self._mData[rowNum]
end

--- 请顺序填充, 不然有问题
---@param row any
---@param col any
---@param data any
function CSV:setCell(row, col, data)
    self._mData[row] = self._mData[row] or {}
    local cell = self._mData[row][col]
    if cell then
        cell:setData(data)
    else
        self._mData[row][col] = self:_spawnCell(data)
    end
    self._maxColNumber = (col < self._maxColNumber) and self._maxColNumber or col
end

function CSV:getRowNumber()
    return #self._mData
end

function CSV:_readString()
    local cell = self:_spawnCell()
    local s = {}
    -- 以 " 开头
    if self._mCurrentChar == '"' then
        self:_getNextChar() -- 跳过 "
        while true do
            if self._mCurrentChar == '"' then
                if self:_checkNextChar('"') then
                    self:_getNextChar() -- 跳过第一个 "
                else
                    break
                end
            end
            s[#s + 1] = self._mCurrentChar
            self:_getNextChar()
        end
        self:_getNextChar() -- 跳过 "
    else
        while self._mCurrentChar ~= "," and self._mCurrentChar ~= "\n" do
            s[#s + 1] = self._mCurrentChar
            self:_getNextChar()
        end
    end
    if self._mCurrentChar == "," then
        self:_getNextChar() -- 吞掉 ,
    end
    cell:setData(table.concat(s))
    return cell
end
