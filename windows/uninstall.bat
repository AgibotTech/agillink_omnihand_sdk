@echo off
REM Omnihand 2025 SDK Windows Uninstall Script
setlocal enabledelayedexpansion

set "INSTALL_DIR=C:\omnihand"

REM Check administrator privileges, request elevation if not available
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [INFO] Requesting administrator privileges...
    powershell -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

echo ============================================
echo Omnihand 2025 SDK - Windows Uninstaller
echo ============================================
echo.

REM Remove C:\omnihand\bin from system PATH
powershell -NoProfile -Command "$p = [Environment]::GetEnvironmentVariable('Path', 'Machine'); if ($p -like '*C:\omnihand\bin*') { $new = ($p -split ';' | Where-Object { $_ -ne 'C:\omnihand\bin' }) -join ';'; [Environment]::SetEnvironmentVariable('Path', $new, 'Machine'); Write-Host '[OK] Removed C:\omnihand\bin from system PATH' } else { Write-Host '[SKIP] C:\omnihand\bin not in PATH' }"
echo.

REM Uninstall C++ SDK
if exist "%INSTALL_DIR%" (
    echo Removing: %INSTALL_DIR%
    rmdir /s /q "%INSTALL_DIR%"
    echo [OK] C++ SDK removed
) else (
    echo [SKIP] Not found: %INSTALL_DIR%
)
echo.

REM Uninstall Python package from every found Python version
echo Uninstalling Python package from each Python version...
set "ANY_PY="
for %%v in (310 311 312 313) do (
    if exist "%LOCALAPPDATA%\Programs\Python\Python%%v\python.exe" (
        set "ANY_PY=1"
        echo   Python %%v ^(user^)
        "%LOCALAPPDATA%\Programs\Python\Python%%v\python.exe" -m pip uninstall omnihand -y --quiet 2>nul
        "%LOCALAPPDATA%\Programs\Python\Python%%v\python.exe" -m pip uninstall omnihand_2025 -y --quiet 2>nul
    )
)
for %%v in (310 311 312 313) do (
    if exist "C:\Python%%v\python.exe" (
        set "ANY_PY=1"
        echo   Python %%v ^(system^)
        "C:\Python%%v\python.exe" -m pip uninstall omnihand -y --quiet 2>nul
        "C:\Python%%v\python.exe" -m pip uninstall omnihand_2025 -y --quiet 2>nul
    )
)
where python >nul 2>&1
if !errorlevel! equ 0 (
    set "ANY_PY=1"
    echo   Python ^(PATH^)
    python -m pip uninstall omnihand -y --quiet 2>nul
    python -m pip uninstall omnihand_2025 -y --quiet 2>nul
)
if defined ANY_PY (
    echo [OK] Python package uninstalled from all found interpreters
) else (
    echo [SKIP] No Python found. Manually run: pip uninstall omnihand
)
echo.

echo ============================================
echo Uninstallation Complete
echo ============================================
echo.
pause
endlocal
