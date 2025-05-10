@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
echo 正在重打包
java -jar jar\apktool_2.11.1.jar b temp\app_src -o temp\app_rebuilt.apk
echo 重打包完成

echo 正在对齐
del temp\app_rebuilt_aligned.apk
C:\work\Pokemon\android\build-tools\36.0.0\zipalign.exe -v 4 temp\app_rebuilt.apk temp\app_rebuilt_aligned.apk
echo 对齐完成

echo 正在签名
call C:\work\Pokemon\android\build-tools\36.0.0\apksigner.bat sign ^
  --ks local\app.jks ^
  --ks-key-alias app ^
  --ks-pass pass:123456 ^
  --key-pass pass:123456 ^
  temp\app_rebuilt_aligned.apk
echo 签名完成

echo 签名校验
call C:\work\Pokemon\android\build-tools\36.0.0\apksigner.bat verify --verbose temp\app_rebuilt_aligned.apk
echo 校验完成

popd
pause

