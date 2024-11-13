@echo off

set "SCRIPT_DIR=Server"

call "%SCRIPT_DIR%\.venv\Scripts\deactivate.bat"

set /p PID=<%SCRIPT_DIR%\python_pid.txt

del %SCRIPT_DIR%\python_pid.txt

@taskkill /f /pid %PID%

