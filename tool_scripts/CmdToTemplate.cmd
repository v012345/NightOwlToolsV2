@echo off
chcp 65001 >nul
cls

setlocal EnableDelayedExpansion
if exist "cmd\传奇-上传数字资源.cmd" copy /Y "cmd\传奇-上传数字资源.cmd" "传奇\上传数字资源.cmd.template"
if exist "cmd\传奇-更新数字资源.cmd" copy /Y "cmd\传奇-更新数字资源.cmd" "传奇\更新数字资源.cmd.template"
if exist "cmd\通用-005-删除指定类型的文件.cmd" copy /Y "cmd\通用-005-删除指定类型的文件.cmd" "通用\通用-005-删除指定类型的文件.cmd.template"
endlocal
