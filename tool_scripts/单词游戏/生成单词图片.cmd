@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion
@REM ="res://assets/words/jp_word_" & A490 & ".png"
py -3 Python\genWordImgByCsv.py --assets_path "C:\NightOwlZone\Client\7city"
py -3 Python\csvToGodotDB.py --csv "单词游戏\words.csv" --gd "C:\NightOwlZone\Client\7city\scripts\db\words\japanese.gd"


endlocal
popd

pause