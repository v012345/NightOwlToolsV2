@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
py -3 Python\engineFilesByCompileCommands.py -c C:\work\BlackMoon\BlackMoonAndroid\engine\.cxx\tools\debug\arm64-v8a\compile_commands.json -t temp\engine_files.txt
popd

pause
