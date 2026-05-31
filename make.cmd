@echo off
setlocal

set "MN_MAKE=%LOCALAPPDATA%\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\mingw32-make.exe"

if not exist "%MN_MAKE%" (
    echo mingw32-make.exe not found at:
    echo   %MN_MAKE%
    exit /b 1
)

"%MN_MAKE%" %*
