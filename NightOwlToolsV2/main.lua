require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Common"
    require "PublishRes.PublishRes"
    require "Translation.Translation"
    local socket = require "socket"
    -- 创建共享数据结构
    local data_queue = {}

    -- 锁，用于控制访问共享数据
    local mutex = false

    -- 生产者协程：接收 socket 数据
    local producer = coroutine.create(function(client)
        while true do
            local data, err = Socket.get_utf8_string(client)
            if data then
                -- 等待锁释放
                while mutex do
                    coroutine.yield()
                end
                if data == "ping" then -- 心跳
                    Socket.put_utf8_string(client, "pong")
                    Socket.put_utf8_string(client, "pong")
                    Socket.put_utf8_string(client, "pong")
                else
                    -- 锁住数据结构并存储数据
                    mutex = true
                    table.insert(data_queue, rec)
                    mutex = false
                end
                -- 通知消费者
                coroutine.yield()
            elseif err == "closed" then
                break
            end
        end
    end)

    -- 消费者协程：处理数据
    local consumer = coroutine.create(function(client)
        while true do
            -- 等待锁释放
            while mutex do
                coroutine.yield()
            end

            -- 锁住数据结构并取出数据
            mutex = true
            local data = table.remove(data_queue, 1)
            mutex = false

            if data then
                print("Processing data:", data)
                client:send("Processing data:" .. data)
            else
                -- 如果没有数据，等待生产者提供
                coroutine.yield()
            end
        end
    end)
    -- 创建一个 TCP 套接字并绑定到本地 IP 地址和端口
    local server = assert(socket.bind("127.0.0.1", 12345))
    -- 设置超时时间为无限，表示服务器会一直等待客户端连接
    server:settimeout(nil)
    -- 等待客户端连接
    local client = server:accept()
    -- 设置客户端的超时时间为 10 秒
    client:settimeout(10)

    -- 主循环
    while true do
        -- 运行生产者协程
        if coroutine.status(producer) ~= "dead" then
            coroutine.resume(producer, client)
        elseif #data_queue <= 0 then
            break
        end

        -- 运行消费者协程
        if coroutine.status(consumer) ~= "dead" then
            coroutine.resume(consumer, client)
        end

        -- 暂停一下防止 CPU 占用过高
        socket.sleep(0.01)
    end
end, function(msg)
    print(msg)
end)
