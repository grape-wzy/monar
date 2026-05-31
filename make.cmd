@echo off
setlocal

set "MN_MAKE="
set "MN_MAKE_DIR="

where /q mingw32-make.exe
if %ERRORLEVEL%==0 (
    set "MN_MAKE=mingw32-make.exe"
)

if not defined MN_MAKE (
    set "MN_MAKE=%LOCALAPPDATA%\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\mingw32-make.exe"
    set "MN_MAKE_DIR=%LOCALAPPDATA%\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
)

if not exist "%MN_MAKE%" if /I not "%MN_MAKE%"=="mingw32-make.exe" (
    echo mingw32-make.exe not found on PATH or at:
    echo   %MN_MAKE%
    exit /b 1
)

if defined MN_MAKE_DIR (
    set "PATH=%MN_MAKE_DIR%;%PATH%"
)

"%MN_MAKE%" %*
