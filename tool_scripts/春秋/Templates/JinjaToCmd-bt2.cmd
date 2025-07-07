@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd ..\..

setlocal EnableDelayedExpansion
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "春秋\Templates\把 svn 的 code 复现到 git 中.cmd.j2" --cmd "春秋\把 svn 的 code 复现到 git 中.cmd"
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "春秋\Templates\把 svn 的 res 复现到 git 中.cmd.j2" --cmd "春秋\把 svn 的 res 复现到 git 中.cmd"
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "春秋\Templates\把 svn 的 execution 复现到 git 中.cmd.j2" --cmd "春秋\把 svn 的 execution 复现到 git 中.cmd"
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "春秋\Templates\检查同名文件是不是一样的.cmd.j2" --cmd "春秋\检查同名文件是不是一样的.cmd"
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "春秋\Templates\对两个文件夹中文件对比.cmd.j2" --cmd "春秋\对两个文件夹中文件对比.cmd"
py -3 Python\JinjaToCmd.py --config_file "春秋\config.ini" --config bt2 --j2 "春秋\Templates\同步 git 到 svn 中.cmd.j2" --cmd "春秋\同步 git 到 svn 中.cmd"
endlocal
popd


