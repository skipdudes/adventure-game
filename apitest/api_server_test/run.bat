@echo off

set "SCRIPT_DIR=..\apitest\api_server_test"

call "%SCRIPT_DIR%\.venv\Scripts\activate.bat"

start /b python "..\apitest\api_server_test\main.py"
for /f "tokens=2" %%i in ('tasklist ^| find "python.exe"') do (
    echo %%i > %SCRIPT_DIR%\python_pid.txt
)
