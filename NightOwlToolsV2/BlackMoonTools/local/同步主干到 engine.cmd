@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
..\lua.exe lua\getFilesOfDir.lua "engine\src\main\cpp\cocos2d" "temp\engine_cocos2d.txt"
..\lua.exe lua\copyFileByTxt.lua "C:\work\BlackMoon\Closers.cocos\client\trunk\cocos2d" "engine\src\main\cpp\cocos2d" "temp\engine_cocos2d.txt"

..\lua.exe lua\getFilesOfDir.lua "engine\src\main\cpp\Classes" "temp\engine_Classes.txt"
..\lua.exe lua\copyFileByTxt.lua "C:\work\BlackMoon\Closers.cocos\client\trunk\Classes" "engine\src\main\cpp\Classes" "temp\engine_Classes.txt"

@REM 这个我也不知道是在哪里拿过来的, 太乱了
..\lua.exe lua\getFilesOfDir.lua "engine\src\main\cpp\aonesdkc" "temp\engine_aonesdkc.txt"
..\lua.exe lua\copyFileByTxt.lua "C:\work\BlackMoon\Closers.android-studio\Project\aonesdkc" "engine\src\main\cpp\aonesdkc" "temp\engine_aonesdkc.txt"

..\lua.exe lua\getFilesOfDir.lua "engine\src\main\cpp\closers" "temp\engine_closers.txt"
..\lua.exe lua\copyFileByTxt.lua "C:\work\BlackMoon\Closers.cocos\client\trunk\proj.android\jni\closerscpp" "engine\src\main\cpp\closers" "temp\engine_closers.txt"
popd

pause
