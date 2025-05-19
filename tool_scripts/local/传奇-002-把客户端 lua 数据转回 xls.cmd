@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion
set serverDir=C:\work\牛马沉默\MirServer
@REM set clientDir=C:\work\Legend\client
set clientDir=C:\work\牛马沉默\客户端

if not exist temp\xls_origin mkdir temp\xls_origin
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_att_score.xls"  "temp\xls_origin\cfg_att_score.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_bubble.xls"  "temp\xls_origin\cfg_bubble.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_chat_drop.xls"  "temp\xls_origin\cfg_chat_drop.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_custpro_caption.xls"  "temp\xls_origin\cfg_custpro_caption.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_mapdesc.xls"  "temp\xls_origin\cfg_mapdesc.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_redpoint.xls"  "temp\xls_origin\cfg_redpoint.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_sound.xls"  "temp\xls_origin\cfg_sound.xls" >nul
@REM copy /Y "%serverDir%\Mir200\Envir\Data\cfg_magicinfo.xls"  "temp\xls_origin\cfg_magicinfo.xls" >nul
@REM copy /Y  "%serverDir%\Mir200\Envir\Data\cfg_skill_present.xls"  "temp\xls_origin\cfg_skill_present.xls" >nul
@REM copy /Y  "%serverDir%\表结构\可视化配置表\cfg_auction_type.xls"  "temp\xls_origin\cfg_auction_type.xls" >nul
@REM copy /Y  "%serverDir%\表结构\可视化配置表\cfg_damage_number.xls"  "temp\xls_origin\cfg_damage_number.xls" >nul
@REM copy /Y  "%serverDir%\表结构\可视化配置表\cfg_setup.xls"  "temp\xls_origin\cfg_setup.xls" >nul
@REM copy /Y  "%serverDir%\表结构\可视化配置表\cfg_game_data.xls"  "temp\xls_origin\cfg_game_data.xls" >nul
@REM copy /Y  "%serverDir%\Mir200\Envir\Data\cfg_model_info.xls"  "temp\xls_origin\cfg_model_info.xls" >nul
@REM copy /Y  "%serverDir%\Mir200\Envir\Data\cfg_buff.xls"  "temp\xls_origin\cfg_buff.xls" >nul

@REM copy /Y  "%serverDir%\Mir200\Envir\Data\cfg_PulsDesc.xls"  "temp\xls_origin\cfg_PulsDesc.xls" >nul

set t[1]=cfg_att_score
set t[2]=cfg_bubble
set t[3]=cfg_chat_drop
set t[4]=cfg_custpro_caption
set t[5]=cfg_mapdesc
set t[6]=cfg_redpoint
set t[7]=cfg_sound
set t[8]=cfg_magicinfo
set t[9]=cfg_skill_present
set t[10]=cfg_auction_type
set t[11]=cfg_damage_number
set t[12]=cfg_setup
set t[13]=cfg_game_data
set t[14]=cfg_model_info
set t[15]=cfg_buff
set t[16]=cfg_PulsDesc
set count=16

if not exist temp\996head mkdir temp\996head
if not exist temp\server mkdir temp\server
for /L %%i in (1,1,%count%) do (
  set t=!t[%%i]!
  bin\lua.exe lua\996genTableHeadForPy.lua "M2Data.!t!" "temp\996head\!t!.py" "!t!"
  py -3 Python\996XlsAllDataToLua.py --xls "temp\xls_origin\!t!.xls" --lua "temp\server\!t!.lua"
)

@REM bin\lua.exe lua\copyFilesToWhere.lua "%clientDir%\dev\scripts\game_config" "temp\lua_origin"

for /L %%i in (1,1,%count%) do (
  set t=!t[%%i]!
  bin\lua.exe lua\996MergeLuaTable.lua "temp.server.!t!" "temp.lua_origin.!t!" "M2Data.!t!" "!t!" "temp\merged\!t!.py"
)

echo "请先处理 temp\merged 下的 py 文件, 一般是删除多余注释"
@REM pause
if exist temp\996xls rd /S /Q temp\996xls
mkdir temp\996xls
for /L %%i in (1,1,%count%) do (
  set t=!t[%%i]!
  py -3 Python\996PyListsToXls.py --mod "temp.merged.!t!" --to "temp\996xls\!t!.xls" --name "!t!"
)

endlocal
popd

pause
