@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\imageToTxt.py --image temp\page_230.png --text temp\page_230.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"


endlocal
popd

pause