---@class Log
Log = {}
setmetatable(Log, {
    __call = function(self, str)
        if not self.logFile then
            self.logFile = io.open("Log.txt", "w")
        end
        self.logFile:write(str)
        return self
    end
})

