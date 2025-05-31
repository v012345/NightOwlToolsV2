@echo off
chcp 65001 >nul
cls
pushd "%~dp0"

cd ..

py -3 "Python\getCosFilesOfDir.py" ^
--config "config\BlackMoonBt5.config" ^
--cnd_folder "dev/" ^
--output_to "temp\cos_files.json"

py -3 "Python\996CheckResFiles.py" ^
--cos_files "temp\cos_files.json" ^
--local_file "C:\work\Legend\996M2_client\digital_res.json" ^
--output_to "temp\cos_miss_files.txt"

popd

pause
