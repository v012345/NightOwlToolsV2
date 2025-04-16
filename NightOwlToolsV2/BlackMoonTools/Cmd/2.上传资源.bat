@REM 就是对客户端的 Resources 里的文件进行处理
@REM 首先要保证 Resources 最新和干净, 自己手动更新与还原吧

@echo off
chcp 65001 >nul
cls

for /f "delims=" %%i in ('powershell -command "Get-Date -Format yyyyMMddHHmmss"') do set TIME_STR=%%i

@echo off
pushd "%~dp0"
@REM py脚本都在 Python 文件夹里, 运行目录就是 /BlackMoonTools
@REM --from_dir "C:\work\BlackMoon\Closers.cocos\client\branches\yougubt5\Resources" ^
@REM --from_dir "C:\Users\Meteor\Desktop\temp\Resources" ^
cd ..

py -3 "Python\UploadHotfixToCos.py" ^
    --compressed_res "C:\Users\Meteor\Desktop\temp\yougubt5\compressed_android" ^
    --config "config\BlackMoonBt5HotfixCosBucket.config" ^
    --cnd_folder "BlackMoonBT5\%TIME_STR%\Android"

py -3 "Python\UploadHotfixToCos.py" ^
    --compressed_res "C:\Users\Meteor\Desktop\temp\yougubt5\compressed_common" ^
    --config "config\BlackMoonBt5HotfixCosBucket.config" ^
    --cnd_folder "BlackMoonBT5\%TIME_STR%\Android"

popd

echo https://c-hy21579qd2zf.648sy.com/BlackMoonBT5/%TIME_STR%/Android/list.csv.gz
@REM echo https://c-hy21579qd2zf.648sy.com/BlackMoonBT5/%TIME_STR%/IOS/list.csv.gz
pause
