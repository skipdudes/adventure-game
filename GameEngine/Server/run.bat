@echo off

set "SCRIPT_DIR=Server"

call "%SCRIPT_DIR%\.venv\Scripts\activate.bat"

start /b python "%SCRIPT_DIR%\main.py"
for /f "tokens=2" %%i in ('tasklist ^| find "python.exe"') do (
    echo %%i > %SCRIPT_DIR%\python_pid.txt
)
