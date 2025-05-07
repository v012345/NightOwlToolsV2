rem NOTV2 == NightOwlToolsV2
@echo off
rem 设置相对路径的源文件和目标文件夹
set source_file=%~dp0build\bin\Release\NightOwlToolsV2.exe
set destination_folder=%~dp0NightOwlToolsV2

rem 直接返回（如果源文件不存在）
if not exist "%source_file%" (
    echo %source_file% not exist
    exit /b
)

rem 创建目标文件夹（如果不存在）
if not exist "%destination_folder%" (
    mkdir "%destination_folder%"
)

rem 移动文件并强制覆盖
xcopy /Y "%source_file%" "%destination_folder%\"

rem 显示完成消息
echo %source_file% move succeed %destination_folder%
@REM pause