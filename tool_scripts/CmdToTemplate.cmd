@echo off
chcp 65001 >nul
cls

setlocal EnableDelayedExpansion
if exist "cmd\传奇-上传数字资源.cmd" copy /Y "cmd\传奇-上传数字资源.cmd" "传奇\上传数字资源.cmd.template"
if exist "cmd\传奇-更新数字资源.cmd" copy /Y "cmd\传奇-更新数字资源.cmd" "传奇\更新数字资源.cmd.template"
if exist "cmd\传奇-把客户端 lua 数据转回 xls.cmd" copy /Y "cmd\传奇-把客户端 lua 数据转回 xls.cmd" "传奇\把客户端 lua 数据转回 xls.cmd.template"
if exist "cmd\传奇-更新策划表到客户端.cmd" copy /Y "cmd\传奇-更新策划表到客户端.cmd" "传奇\更新策划表到客户端.cmd.template"
if exist "cmd\通用-删除指定类型的文件.cmd" copy /Y "cmd\通用-删除指定类型的文件.cmd" "通用\删除指定类型的文件.cmd.template"
if exist "cmd\通用-获取文件夹下文件的大小.cmd" copy /Y "cmd\通用-获取文件夹下文件的大小.cmd" "通用\获取文件夹下文件的大小.cmd.template"
if exist "cmd\通用-对比 lua 表是否相同.cmd" copy /Y "cmd\通用-对比 lua 表是否相同.cmd" "通用\获对比 lua 表是否相同.cmd.template"
endlocal
