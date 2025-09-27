@REM json 是使用 chatgpt , deepseek 等中图片里提取出来的
@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\word_json_to_csv.py --json_path "单词游戏\N2单词2500.json" --example_path "单词游戏\example.csv" --japanese_path "单词游戏\japanese.csv" --book_id 1


endlocal
popd

pause