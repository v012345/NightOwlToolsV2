@REM json 是使用 chatgpt , deepseek 等中图片里提取出来的
@REM [    
@REM     {
@REM         "id": 274,
@REM         "kana": "かかす",
@REM         "kanji": "欠かす",
@REM         "chinese": "缺，停止",
@REM         "example": [
@REM             [
@REM                 "毎朝欠かさず、ジョギングをする。",
@REM                 "每天早上不停歇地跑步。"
@REM             ]
@REM         ]
@REM     }
@REM ]

@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\word_json_to_csv.py --json_path "单词游戏\N2单词2500.json" --japanese_path "单词游戏\japanese.csv" --example_path "单词游戏\example.csv"  --book_id 1


endlocal
popd

pause