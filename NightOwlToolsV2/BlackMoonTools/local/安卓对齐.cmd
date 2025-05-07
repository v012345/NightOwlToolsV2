@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
del temp\app_rebuilt_aligned.apk
C:\work\Pokemon\android\build-tools\36.0.0\zipalign.exe -v 4 temp\app_rebuilt.apk temp\app_rebuilt_aligned.apk
popd
pause

