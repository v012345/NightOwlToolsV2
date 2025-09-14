@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\pdfToImage.py


endlocal
popd

pause