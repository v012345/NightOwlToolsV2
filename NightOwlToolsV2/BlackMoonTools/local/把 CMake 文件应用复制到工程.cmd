@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
..\NightOwlToolsV2.exe lua\copyFilesToWhere.lua "engine" "C:\work\BlackMoon\BlackMoonAndroid\engine" 

popd

pause
