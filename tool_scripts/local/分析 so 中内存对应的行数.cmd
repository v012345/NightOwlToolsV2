@echo off
chcp 65001 > nul
cls
pushd "%~dp0"
cd ..
C:\work\Pokemon\android\ndk\27.0.12077973\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-addr2line.exe ^
-e C:\Users\Meteor\Documents\leidian9\Pictures\libclosers.so 0x00007fff6c8a8cb1

C:\work\Pokemon\android\ndk\27.0.12077973\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-readelf.exe -S ^
C:\Users\Meteor\Documents\leidian9\Pictures\libclosers.so
popd
pause
