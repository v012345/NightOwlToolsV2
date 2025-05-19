@echo off
chcp 65001 >nul
cls

setlocal EnableDelayedExpansion

copy /Y "传奇\上传数字资源.cmd.template"  "cmd\传奇-上传数字资源.cmd"
copy /Y "传奇\更新数字资源.cmd.template"  "cmd\传奇-更新数字资源.cmd"

endlocal
