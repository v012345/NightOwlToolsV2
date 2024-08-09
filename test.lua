xpcall(function()
    print("cheers! start!")
    local socket = require("socket.core")
    for f in lfs.dir(".") do
        print(f)
    end


    local handle, param = Common.CreateThread([[
        local socket = require "socket.core"
        for i = 1, 2, 1 do
            print("child thread "..i)
            socket.sleep(1)
        end
        return "ajflsejslgjoijfoisej"
        ]])



    while not Common.WaitForSingleObject(handle) do
        socket.sleep(0.5)
        print("main thread")
        -- local shell = io.popen(publist_cmd) or error("can't execute " .. publist_cmd)
        -- result = shell:read("a")
        -- shell:close()
    end

    -- if string.find(result, "Publish success!") then
    --     Common.Write(string.format("Publish success! spent %ss", os.time() - start_time))
    -- else
    --     error(result)
    -- end
    print(Common.GetExitCodeThread(handle, param))
    Common.CloseHandle(handle)
    print("?????")
    print(sqlite3)
    print("cheers! end!")
end, function(msg)
    print(msg)
end)
