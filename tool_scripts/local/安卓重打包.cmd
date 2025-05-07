@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
java -jar jar\apktool_2.11.1.jar b temp\app_src -o temp\app_rebuilt.apk
popd
pause

