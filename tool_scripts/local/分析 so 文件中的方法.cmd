@REM C:\work\Pokemon\android\ndk\27.0.12077973\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-nm.exe .\build\intermediates\cxx\Debug\3k1u1i3h\obj\x86\libclosers.so | findstr Java_ >> .\build\native1.log
@REM C:\work\Pokemon\android\ndk\27.0.12077973\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-nm.exe .\build\intermediates\cmake\debug\obj\x86\libclosers.so | findstr Java_ >> .\build\native2.log

@REM C:\work\Pokemon\android\ndk\27.0.12077973\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-nm.exe .\build\intermediates\cxx\Debug\3k1u1i3h\obj\x86\libclosers.so  >> .\build\x86.log
@echo off
chcp 65001 >nul
cls
pushd "%~dp0"
cd ..

C:\work\Pokemon\android\ndk\27.0.12077973\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-nm.exe ^
 C:\Users\Meteor\Documents\leidian9\Pictures\libclosers.so ^
 >> temp\libclosers.txt
popd
pause

