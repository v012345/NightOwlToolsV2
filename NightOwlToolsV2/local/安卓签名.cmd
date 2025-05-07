@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
echo 正在签名
call C:\work\Pokemon\android\build-tools\36.0.0\apksigner.bat sign ^
  --ks local\app.jks ^
  --ks-key-alias app ^
  --ks-pass pass:123456 ^
  --key-pass pass:123456 ^
  temp\app_rebuilt_aligned.apk
echo 签名完成

@REM echo 签名校验
@REM call C:\work\Pokemon\android\build-tools\36.0.0\apksigner.bat verify --verbose temp\app_rebuilt_aligned.apk
@REM echo 校验完成
popd
pause

