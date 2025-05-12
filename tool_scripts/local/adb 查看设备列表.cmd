@echo off
chcp 65001 >nul
cls
pushd "%~dp0"
setlocal
cd ..
set adb=C:\work\Pokemon\android\platform-tools\adb.exe

@REM 查看已经连接设置
%adb% devices
@REM %adb% root
%adb% -s 8c5d4da7 shell
endlocal

popd
pause
