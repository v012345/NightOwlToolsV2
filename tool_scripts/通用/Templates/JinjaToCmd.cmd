@echo off
chcp 65001 > nul
cls
pushd "%~dp0"
cd ..\..
setlocal EnableDelayedExpansion
py -3 Python\JinjaToCmd.py --j2 "通用\Templates\测试用.cmd.j2" --cmd "通用\测试用.cmd"
endlocal
popd
