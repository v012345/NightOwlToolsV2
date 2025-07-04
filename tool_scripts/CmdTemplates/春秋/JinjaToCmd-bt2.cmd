@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd ..\..

setlocal EnableDelayedExpansion
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "CmdTemplates\春秋\把 svn 的 code 复现到 git 中.cmd.j2" --cmd "春秋\把 svn 的 code 复现到 git 中.cmd"
endlocal
popd


