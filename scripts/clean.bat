@echo off
setlocal

rem Remove the 'build' directory in the project root (this script's directory)
cd /d "%~dp0"

set "TARGET_DIR=../build"

if not exist "%TARGET_DIR%\" (
  echo No 'build' directory found in "%CD%".
  exit /b 0
)

echo Removing "%CD%\%TARGET_DIR%" ...
rmdir /s /q "%TARGET_DIR%"
if exist "%TARGET_DIR%\" (
  echo Failed to remove "%CD%\%TARGET_DIR%".
  exit /b 1
)

echo Done.
exit /b 0