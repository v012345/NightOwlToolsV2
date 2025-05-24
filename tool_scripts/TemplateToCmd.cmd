@echo off
chcp 65001 >nul
cls

setlocal EnableDelayedExpansion

copy /Y "传奇\上传数字资源.cmd.template"  "cmd\传奇-上传数字资源.cmd"
copy /Y "传奇\更新数字资源.cmd.template"  "cmd\传奇-更新数字资源.cmd"
copy /Y "传奇\删除数字资源后请使用.cmd.template"  "cmd\传奇-删除数字资源后请使用.cmd"
copy /Y "传奇\把客户端 lua 数据转回 xls.cmd.template"  "cmd\传奇-把客户端 lua 数据转回 xls.cmd"
copy /Y "传奇\更新策划表到客户端.cmd.template"  "cmd\传奇-更新策划表到客户端.cmd"
copy /Y "通用\删除指定类型的文件.cmd.template"  "cmd\通用-删除指定类型的文件.cmd"
copy /Y "通用\获取文件夹下文件的大小.cmd.template"  "cmd\通用-获取文件夹下文件的大小.cmd"
copy /Y "通用\对比 lua 表是否相同.cmd.template"  "cmd\通用-对比 lua 表是否相同.cmd"
copy /Y "传奇\提取传奇官方 lua 文件.cmd.template"  "cmd\传奇-提取传奇官方 lua 文件.cmd"
copy /Y "通用\获得文件夹下的某类文件.cmd.template"  "cmd\通用-获得文件夹下的某类文件.cmd"

endlocal
