xpcall(function()
    print("cheers! start!")
    for f in lfs.dir(".") do
        print(f)
    end
    print("cheers! end!")
end, function(msg)
    print(msg)
end)
