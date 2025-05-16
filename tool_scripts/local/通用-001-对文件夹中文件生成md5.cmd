就是生成一个 文件名 + md5 的 csv
@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion

set LUA_PARAM=return { ^
exclude = { '.git', '.svn', '.vscode', 'dev', 'no_run' } ^
}

bin\lua.exe lua\genSha1OfDir.lua "C:\work\Legend\M2_241107" "temp\common_001.csv" "!LUA_PARAM!"

endlocal
popd

pause
