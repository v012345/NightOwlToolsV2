@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion

set LUA_PARAM="return { exclude = { ['.git'] = true, ['.svn'] = true, ['.vscode'] = true, ['dev'] = true, ['no_run'] = true } , ext = { ['.lua'] = true }}"

bin\lua.exe lua\getFilesOfDirWithConfig.lua "C:\work\Legend\client" "temp\996lua.txt" !LUA_PARAM!
endlocal
popd

pause
