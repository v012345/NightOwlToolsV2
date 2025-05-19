@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion

bin\lua.exe lua\getFilesOfDir.lua "C:\work\牛马沉默\客户端\dev" "temp\common_004_1.txt"
@REM bin\lua.exe lua\getFileSizeByTxt.lua "C:\Users\Meteor\Desktop\client\dev" "temp\common_004_2.txt" "temp\common_004_1.txt" 

endlocal
popd

pause
