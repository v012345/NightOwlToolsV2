@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
C:\Users\MH\.jdks\corretto-1.8.0_462\bin\java.exe -jar jar\apktool_2.11.1.jar d temp\v1.0.0_release_yougubt4_res-res_zhcn_smartspace_202509180047_yougu_andbt4_2509180526.apk -f -o temp\app_src
popd
pause

