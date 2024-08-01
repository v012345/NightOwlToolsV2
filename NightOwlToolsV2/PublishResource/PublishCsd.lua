local db = sqlite3.open('LocalOnly/BlackMoon.db')
for i, RootFolders in ipairs(PublishResource.RootFolders) do
    local RootFolder = RootFolders.from
    local uiTableNmae = "ui" .. Common.EasyChecksum(RootFolder)
    PublishResource.TouchTable(db, uiTableNmae)
    local uiFolder = RootFolder .. "/cocosstudio/ui"
    local uiRoot = RootFolder .. "/cocosstudio"
    local uiRootLen = #uiRoot + 2
    local uiFiles = PublishResource.GetFilesCurrentState(uiFolder, ".csd")
    local toPublish = {}
    local lastState = {}
    local nowState = {}
    for i, v in ipairs(uiFiles) do
        local relative_path = string.sub(v.path, uiRootLen)
        nowState[relative_path] = {
            name = v.name,
            modification = v.modification,
            relative_path = relative_path,
            path = v.path
        }
    end
    for row in db:nrows("SELECT * FROM " .. uiTableNmae) do
        lastState[row.relative_path] = {
            id = row.id,
            name = row.name,
            sha1 = row.sha1,
            modification = row.modification,
            relative_path = row.relative_path,
            path = uiRoot .. "/" .. row.relative_path
        }
    end

    for k, v in pairs(nowState) do
        local last = lastState[k]
        if last then
            if (last.modification < v.modification and last.sha1 ~= Win32.getSha1(v.path)) or PublishResource.PublishAll then
                toPublish[#toPublish + 1] = last
            end
        else
            toPublish[#toPublish + 1] = v
        end
    end

    local publishNum = #toPublish
    if publishNum > 0 then
        local stmt = db:prepare(string.format([[ INSERT OR REPLACE INTO 
                  %s (id,modification,sha1,name,relative_path)
                  VALUES (:id,:modification,:sha1,:name,:relative_path)]], uiTableNmae))

        for i, v in ipairs(toPublish) do
            Common.ShowOnOneline(string.format("update db %s/%s %s", i, publishNum, v.name))
            stmt:bind_names({
                id = v.id,
                modification = v.modification,
                sha1 = Win32.getSha1(v.path),
                name = v.name,
                relative_path = v.relative_path
            })
            stmt:step()
            stmt:reset()
        end
        stmt:finalize()
        print()
        ---@type XML
        local temp_css = XML [[
    <Solution>
        <PropertyGroup Name="Closers" Version="3.10.0.0" Type="CocosStudio" />
        <SolutionFolder>
            <Group ctype="ResourceGroup">
                <RootFolder Name=".">
                    <Folder Name="ui">
                    </Folder>
                </RootFolder>
            </Group>
        </SolutionFolder>
    </Solution>
    ]]
        ---@type XMLNode
        local ui_node = temp_css:getRootNode():getChild(2):getChild(1):getChild(1):getChild(1)
        --  <Project Name="UiAccessTypeBuy.csd" Type="Layer" />
        for i, v in ipairs(toPublish) do
            ---@type XMLNode
            local newNode = XML:newNode("Project")
            newNode:setAttributeValue("Name", v.name)
            newNode:setAttributeValue("Type", "Layer")
            ui_node:addChild(newNode)
        end
        local temp_css_file = RootFolder .. "/Closers_temp1.ccs"
        temp_css:writeTo(temp_css_file)

        local cocos_cmd = '"%s" publish -f %s -o %s -s -d Serializer_FlatBuffers'
        local cmd = string.format(cocos_cmd, PublishResource.CocosTool, temp_css_file, RootFolders.to)
        local exe_cmd = io.popen(cmd) or error("can't execute " .. cocos_cmd)
        print(exe_cmd:read("a"))
        exe_cmd:close()
        os.remove(temp_css_file)
    else
        print(string.format("update %s files", publishNum))
    end

end
db:close()
