@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion
if not exist temp\dev mkdir temp\dev
if not exist temp\996head mkdir temp\996head

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

:: 遍历数组
for /L %%i in (1,1,%count%) do (
    set t=!t[%%i]!
    bin\lua.exe lua\996genTableHeadForPy.lua "M2Data.!t!" "temp\996head\!t!.py" "!t!"
    py -3 Python\996XlsToClientLua.py --xls "temp\996xls\!t!.xls" --lua "temp\dev\!t!.lua" --head "temp.996head.!t!" --name "!t!"
)

endlocal
popd

pause
