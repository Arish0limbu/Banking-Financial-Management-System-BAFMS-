@echo off
cd /d "%~dp0"

echo.
echo Compiling BAFMS...
echo.

g++ -std=c++17 main.cpp admin.cpp account.cpp customer.cpp design.cpp employee.cpp finance.cpp loan.cpp security.cpp transaction.cpp -o banking.exe

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
