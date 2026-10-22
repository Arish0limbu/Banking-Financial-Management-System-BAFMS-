@echo off
setlocal EnableDelayedExpansion

cd /d "%~dp0"

set "files="

for /r %%f in (*.cpp) do (
set "files=!files! "%%f""
)

if not defined files (
echo No C++ source files found!
pause
exit /b 1
)

echo.
echo Compiling BAFMS...
echo.

g++ -std=c++17 -I include %files% -o banking.exe

if errorlevel 1 (
echo.
echo Compilation failed!
pause
exit /b 1
)

echo.
echo Compilation successful!
echo Starting BAFMS...
echo.

banking.exe

pause
