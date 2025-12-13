@echo off
REM build.bat - Automates CMake build process (Release mode)

REM Create build directory if it doesn't exist
if not exist build mkdir build

REM Configure project with CMake (Release mode)
cmake -B build -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed.
    exit /b %ERRORLEVEL%
)

REM Build project
cmake --build build --config Release
if %ERRORLEVEL% neq 0 (
    echo Build failed.
    exit /b %ERRORLEVEL%
)

echo.
echo Build complete. Executable: build/Release/raytracer.exe
