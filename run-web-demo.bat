@echo off
cd /d "%~dp0"

echo.
echo Building the local BAFMS web demo...
echo.

g++ -std=c++17 web_api.cpp account.cpp transaction.cpp design.cpp -o web-api.exe

if errorlevel 1 (
    echo.
    echo Web demo build failed. Check that MinGW g++ is installed and available on PATH.
    pause
    exit /b 1
)

where node >nul 2>nul
if errorlevel 1 (
    echo Node.js is required to serve the local web page.
    pause
    exit /b 1
)

echo.
echo Starting at http://127.0.0.1:4173
echo Keep this window open while using the demo. Press Ctrl+C to stop.
echo.
node web-demo/server.js
