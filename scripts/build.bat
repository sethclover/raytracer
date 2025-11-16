@echo off
REM build.bat - Automates CMake build process

REM Create build directory if it doesn't exist
if not exist build mkdir build

REM Enter build directory
cd build

REM Run CMake configuration step
cmake ..

REM Build project
cmake --build .

REM Return to project root
cd ..
