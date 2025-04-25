@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
..\NightOwlToolsV2.exe lua\copyDirStruct.lua "C:\work\BlackMoon\Closers.cocos\client\trunk\cocos2d" "temp\cocos2d"
..\NightOwlToolsV2.exe lua\copyMkCmake.lua "C:\work\BlackMoon\Closers.cocos\client\trunk\cocos2d" "temp\cocos2d"
..\NightOwlToolsV2.exe lua\remove_empty_dirs.lua "temp\cocos2d"

popd

pause
