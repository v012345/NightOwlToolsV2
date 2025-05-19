@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion

set LUA_PARAM=return { ^
exclude = { '.git', '.svn', '.vscode' }, ^
ext = { '.xls' } ^
}

@REM "就是生成一个 文件名 + md5 的 csv"
bin\lua.exe lua\getFilesOfDirWithConfig.lua "C:\work\牛马沉默\MirServer" "temp\common_002.txt" "!LUA_PARAM!"

endlocal
popd

pause
