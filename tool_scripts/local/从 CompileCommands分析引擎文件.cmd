@echo off
chcp 65001 >nul
cls

pushd "%~dp0"
setlocal

@REM REM 正确设置 engine 路径（去掉双引号）
set engine=C:\work\BlackMoon\BlackMoonAndroid\engine

@REM REM 收集 debug 中的, 所以要先生成一遍 debug 工程
cd ..

@REM REM 调用 Python 脚本分析 compile_commands.json
@REM py -3 Python\analysisCompileCommands.py ^
@REM     "%engine%\.cxx\tools\debug\arm64-v8a\compile_commands.json" ^
@REM     "%engine%\.cxx\tools\debug\armeabi-v7a\compile_commands.json" ^
@REM     "%engine%\.cxx\tools\debug\x86\compile_commands.json" ^
@REM     "%engine%\.cxx\tools\debug\x86_64\compile_commands.json" ^
@REM     "%engine%\.cxx\tools\debug\riscv64\compile_commands.json" ^
@REM     -t temp\commands_files.txt ^
@REM     -r %engine%
bin\lua.exe lua\copyDirStruct.lua "%engine%" "engine" 
bin\lua.exe lua\copyFileByTxt.lua "%engine%" engine temp\commands_files.txt
@REM 这里没有什么好方法, 只好把 a so CMakeList 都复制过来了
bin\lua.exe lua\copyLibAndCmakeFiles.lua "%engine%" engine
bin\lua.exe lua\remove_empty_dirs.lua "engine"

@REM REM 备用脚本，分析单个 compile_commands.json
@REM REM py -3 Python\engineFilesByCompileCommands.py -c "%engine%\.cxx\tools\debug\arm64-v8a\compile_commands.json" -t temp\engine_files.txt
endlocal 
popd
pause