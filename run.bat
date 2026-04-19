@echo off
if exist "DSA Project2.exe" (
    "DSA Project2.exe"
) else (
    echo Executable not found! Please run build.bat first.
    pause
)
