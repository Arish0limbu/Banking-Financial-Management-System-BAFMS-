@echo off
setlocal EnableDelayedExpansion

set "files="
for /r %%f in (*.cpp) do (
    set "files=!files! "%%f""
)

g++ -std=c++17 -I include %files% -o banking.exe

if %errorlevel% equ 0 banking.exe