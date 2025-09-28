@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion
@REM ="res://assets/words/jp_word_" & A490 & ".png"
@REM 已经不使用了
@REM py -3 Python\genWordImgByCsv.py --assets_path "C:\NightOwlZone\Client\7city"


endlocal
popd

pause