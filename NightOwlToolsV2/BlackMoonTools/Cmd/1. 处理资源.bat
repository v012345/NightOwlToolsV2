@REM 就是对客户端的 Resources 里的文件进行处理
@REM 首先要保证 Resources 最新和干净, 自己手动更新与还原吧

@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
@REM py脚本都在 Python 文件夹里, 运行目录就是 /BlackMoonTools
@REM --from_dir "C:\work\BlackMoon\Closers.cocos\client\branches\yougubt5\Resources" ^
@REM --from_dir "C:\Users\Meteor\Desktop\temp\Resources" ^
cd ..
py -3 "Python\publish_res.py" ^
    --from_dir "C:\work\BlackMoon\Closers.cocos\client\branches\yougubt5\Resources" ^
    --to_dir "C:\Users\Meteor\Desktop\temp\yougubt5" ^
popd

pause
