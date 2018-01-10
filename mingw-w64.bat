echo off
set PATH=%~d0\TOOL\msys2\mingw64\bin;%PATH%
rem echo %PATH%
cd %~dp0
"C:\WINDOWS\system32\cmd.exe" %1 %2 %3 %4
