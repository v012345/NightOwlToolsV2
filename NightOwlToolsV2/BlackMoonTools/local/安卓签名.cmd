@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
call C:\work\Pokemon\android\build-tools\36.0.0\apksigner.bat sign ^
  --ks local\app.jks ^
  --ks-key-alias app ^
  --ks-pass pass:123456 ^
  --key-pass pass:123456 ^
  temp\app_rebuilt_aligned.apk

call C:\work\Pokemon\android\build-tools\36.0.0\apksigner.bat verify --verbose temp\app_rebuilt_aligned.apk
popd
pause

