@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\imageToTxt.py --image temp\page_12.png --text temp\page_12.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
@REM py -3 Python\imageToTxt.py --image temp\page_48.png --text temp\page_48.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
@REM py -3 Python\imageToTxt.py --image temp\page_49.png --text temp\page_49.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
@REM py -3 Python\imageToTxt.py --image temp\page_50.png --text temp\page_50.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
@REM py -3 Python\imageToTxt.py --image temp\page_51.png --text temp\page_51.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
@REM py -3 Python\imageToTxt.py --image temp\page_52.png --text temp\page_52.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"


endlocal
popd

pause