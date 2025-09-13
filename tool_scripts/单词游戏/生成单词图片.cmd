@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\genWordImgByCsv.py --assets_path "C:\NightOwlZone\Client\7city"


endlocal
popd

pause