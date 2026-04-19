@echo off
if not exist "raylib" (
    echo Raylib folder not found! Running setup...
    powershell -ExecutionPolicy Bypass -File setup_raylib.ps1
)

echo Building...
g++ -o "DSA Project2.exe" main.cpp src/*.cpp -Iinclude -Iraylib/include -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static

if %ERRORLEVEL% EQU 0 (
    echo Build Successful! Run 'run.bat' to start the application.
) else (
    echo Build Failed!
    pause
)
