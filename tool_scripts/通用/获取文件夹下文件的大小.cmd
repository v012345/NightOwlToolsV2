@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion

set dir_path=C:\work\ChunQiuFengShenBT2\03_Code\unity3dProj
bin\lua.exe lua\getFilesOfDir.lua "%dir_path%" "temp\文件大小.txt"
bin\lua.exe lua\getFileSizeByTxt.lua "%dir_path%" "temp\文件大小.txt" "temp\文件大小.txt" 

endlocal
popd

pause
