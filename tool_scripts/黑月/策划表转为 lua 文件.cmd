@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
cd ..

setlocal EnableDelayedExpansion



set xls_dir="C:\work\BlackMoon\Closers.cocos\resource\data\branches\yougubt5\excel"
set lua_dir="C:\work\Closers.pc\Resources\src\imports\table"


py -3  Python\ClosersXlsToLua.py --xls_dir !xls_dir! --lua_dir !lua_dir! 


endlocal
popd

pause