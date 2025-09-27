@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\csvToGodotDB.py --csv "单词游戏\japanese.csv" --gd "C:\NightOwlZone\Client\7city\scripts\db\words\japanese.gd"
py -3 Python\csvToGodotDB.py --csv "单词游戏\example.csv" --gd "C:\NightOwlZone\Client\7city\scripts\db\words\example.gd"


endlocal
popd

pause