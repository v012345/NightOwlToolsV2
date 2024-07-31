xpcall(function()
    print("cheers! start!")
    print(require("socket.core"))
    for f in lfs.dir(".") do
        print(f)
    end
    print(sqlite3)
    print("cheers! end!")
end, function(msg)
    print(msg)
end)
