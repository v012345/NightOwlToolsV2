@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion

@REM bin\lua.exe lua\getFilesOfDir.lua "C:\Users\Meteor\Desktop\client\dev" "temp\common_004_1.txt"
bin\lua.exe lua\getFileSizeByTxt.lua "C:\Users\Meteor\Desktop\client\dev" "temp\common_004_2.txt" "temp\common_004_1.txt" 

endlocal
popd

pause
