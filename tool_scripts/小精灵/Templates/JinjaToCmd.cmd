@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd ..\..

setlocal EnableDelayedExpansion
py -3 Python\JinjaToCmd.py --config_file "小精灵\config.ini" --config default --j2 "小精灵\Templates\复制文件到热更目录.cmd.j2" --cmd "小精灵\复制文件到热更目录.cmd"
endlocal
popd


