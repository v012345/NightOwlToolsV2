@echo off
chcp 65001 >nul
cls

setlocal EnableDelayedExpansion
if exist "cmd\传奇-上传数字资源.cmd" copy /Y "cmd\传奇-上传数字资源.cmd" "传奇\上传数字资源.cmd.template"
if exist "cmd\传奇-删除数字资源后请使用.cmd" copy /Y "cmd\传奇-删除数字资源后请使用.cmd" "传奇\删除数字资源后请使用.cmd.template"
if exist "cmd\传奇-把客户端 lua 数据转回 xls.cmd" copy /Y "cmd\传奇-把客户端 lua 数据转回 xls.cmd" "传奇\把客户端 lua 数据转回 xls.cmd.template"
if exist "cmd\传奇-把策划表转为 lua 表.cmd" copy /Y "cmd\传奇-把策划表转为 lua 表.cmd" "传奇\把策划表转为 lua 表.cmd.template"
if exist "cmd\传奇-提取传奇官方 lua 文件.cmd" copy /Y "cmd\传奇-提取传奇官方 lua 文件.cmd" "传奇\提取传奇官方 lua 文件.cmd.template"
if exist "cmd\传奇-更新数字资源.cmd" copy /Y "cmd\传奇-更新数字资源.cmd" "传奇\更新数字资源.cmd.template"
if exist "cmd\传奇-更新策划表到客户端.cmd" copy /Y "cmd\传奇-更新策划表到客户端.cmd" "传奇\更新策划表到客户端.cmd.template"
if exist "cmd\通用- GBK 转为 UTF-8.cmd" copy /Y "cmd\通用- GBK 转为 UTF-8.cmd" "通用\GBK 转为 UTF-8.cmd.template"
if exist "cmd\通用- lua 表 A 是否包含表 B.cmd" copy /Y "cmd\通用- lua 表 A 是否包含表 B.cmd" "通用\lua 表 A 是否包含表 B.cmd.template"
if exist "cmd\通用-删除指定类型的文件.cmd" copy /Y "cmd\通用-删除指定类型的文件.cmd" "通用\删除指定类型的文件.cmd.template"
if exist "cmd\通用-对两个文件夹中文件对比.cmd" copy /Y "cmd\通用-对两个文件夹中文件对比.cmd" "通用\对两个文件夹中文件对比.cmd.template"
if exist "cmd\通用-把 B 表能过键打入 A 表中.cmd" copy /Y "cmd\通用-把 B 表能过键打入 A 表中.cmd" "通用\把 B 表能过键打入 A 表中.cmd.template"
if exist "cmd\通用-把 lua 表转为 xls.cmd" copy /Y "cmd\通用-把 lua 表转为 xls.cmd" "通用\把 lua 表转为 xls.cmd.template"
if exist "cmd\通用-把 xls 转为 lua 表.cmd" copy /Y "cmd\通用-把 xls 转为 lua 表.cmd" "通用\把 xls 转为 lua 表.cmd.template"
if exist "cmd\通用-检查 lua 表的 id.cmd" copy /Y "cmd\通用-检查 lua 表的 id.cmd" "通用\检查 lua 表的 id.cmd.template"
if exist "cmd\通用-获取文件夹下文件的大小.cmd" copy /Y "cmd\通用-获取文件夹下文件的大小.cmd" "通用\获取文件夹下文件的大小.cmd.template"
if exist "cmd\获得文件夹下的某类文件.cmd" copy /Y "cmd\通用-获得文件夹下的某类文件.cmd" "通用\获得文件夹下的某类文件.cmd.template"
if exist "cmd\表 A 是与表 B 的公共部分是否相同.cmd" copy /Y "cmd\通用-表 A 是与表 B 的公共部分是否相同.cmd" "通用\表 A 是与表 B 的公共部分是否相同.cmd.template"
endlocal

