@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion
set client_dir=C:\Users\Meteor\Desktop\client
py -3 Python\996UpdateRes.py  --client "C:\Users\Meteor\Desktop\client" --config "config\BlackMoonBt5.config"
endlocal
popd
pause
