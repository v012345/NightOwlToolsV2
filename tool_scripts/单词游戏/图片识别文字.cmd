@echo off
chcp 65001 > nul
cls

pushd "%~dp0"
cd..

setlocal EnableDelayedExpansion

py -3 Python\imageToTxt.py --image temp\page_232.png --text temp\page_232.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
py -3 Python\imageToTxt.py --image temp\page_234.png --text temp\page_234.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
py -3 Python\imageToTxt.py --image temp\page_236.png --text temp\page_236.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
py -3 Python\imageToTxt.py --image temp\page_238.png --text temp\page_238.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
py -3 Python\imageToTxt.py --image temp\page_240.png --text temp\page_240.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"
py -3 Python\imageToTxt.py --image temp\page_242.png --text temp\page_242.txt --tesseract "C:\Program Files\Tesseract-OCR\tesseract.exe"


endlocal
popd

pause