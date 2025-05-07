@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
..\lua.exe lua\copyDirStruct.lua "C:\work\BlackMoon\BlackMoonAndroid\engine" "engine"
..\lua.exe lua\copyMkCmake.lua "C:\work\BlackMoon\BlackMoonAndroid\engine" "engine"
..\lua.exe lua\remove_empty_dirs.lua "engine"

popd

pause
