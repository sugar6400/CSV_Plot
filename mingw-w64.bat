echo off
set PATH=%~d0\TOOL\mingw-w64\x86_64-7.2.0-posix-seh-rt_v5-rev1\mingw64\bin;%PATH%
rem echo %PATH%
cd %~dp0
"C:\WINDOWS\system32\cmd.exe" %1 %2 %3 %4
