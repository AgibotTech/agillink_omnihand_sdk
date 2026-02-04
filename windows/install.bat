@echo off
REM Omnihand 2025 SDK Windows Install Script
setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
set "INSTALL_DIR=C:\omnihand"

REM Check administrator privileges, request elevation if not available
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [INFO] Requesting administrator privileges...
    powershell -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

echo ============================================
echo Omnihand 2025 SDK - Windows Installer
echo ============================================
echo.
echo Installing to: %INSTALL_DIR%
echo.

REM Install C++ SDK
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
xcopy /E /I /Y "%SCRIPT_DIR%cpp\include" "%INSTALL_DIR%\include" >nul
xcopy /E /I /Y "%SCRIPT_DIR%cpp\lib" "%INSTALL_DIR%\lib" >nul
xcopy /E /I /Y "%SCRIPT_DIR%cpp\bin" "%INSTALL_DIR%\bin" >nul
if exist "%SCRIPT_DIR%cpp\share\cmake\omnihand" (
    xcopy /E /I /Y "%SCRIPT_DIR%cpp\share\cmake\omnihand" "%INSTALL_DIR%\share\cmake\omnihand" >nul
)
if exist "%SCRIPT_DIR%cpp\cmake" (
    xcopy /E /I /Y "%SCRIPT_DIR%cpp\cmake" "%INSTALL_DIR%\cmake" >nul
)
echo [OK] C++ SDK installed to %INSTALL_DIR%
echo.

REM Detect Python and install matching wheel
echo Installing Python package...
set "PYTHON_CMD="
where python >nul 2>&1 && set "PYTHON_CMD=python"
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python310\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python310\python.exe"
)
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python311\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python311\python.exe"
)
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python312\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python312\python.exe"
)
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python313\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python313\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python310\python.exe" set "PYTHON_CMD=C:\Python310\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python311\python.exe" set "PYTHON_CMD=C:\Python311\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python312\python.exe" set "PYTHON_CMD=C:\Python312\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python313\python.exe" set "PYTHON_CMD=C:\Python313\python.exe"
)

if defined PYTHON_CMD (
    echo Using Python: !PYTHON_CMD!
    set "PY_VER="
    for /f "delims=" %%v in ('"!PYTHON_CMD!" -c "import sys; print(str(sys.version_info.major)+str(sys.version_info.minor))" 2^>nul') do set "PY_VER=%%v"
    if defined PY_VER (
        set "WHL_MATCH="
        for %%f in ("%SCRIPT_DIR%python\*-cp!PY_VER!-cp!PY_VER!-*.whl") do (
            set "WHL_MATCH=1"
            echo Installing %%~nxf ^(Python !PY_VER! only^) ...
            "!PYTHON_CMD!" -m pip install "%%f" --force-reinstall --no-deps --quiet
            if !errorlevel! equ 0 (echo [OK] Installed: %%~nxf) else (echo [FAIL] %%~nxf)
        )
        if defined WHL_MATCH (
            echo [OK] Python package installed for Python !PY_VER!
        ) else (
            echo [WARN] No wheel for Python !PY_VER! - trying all wheels...
            set "INSTALLED="
            for %%f in ("%SCRIPT_DIR%python\*.whl") do (
                "!PYTHON_CMD!" -m pip install "%%f" --force-reinstall --no-deps --quiet 2>nul
                if !errorlevel! equ 0 set "INSTALLED=%%~nxf"
            )
            if defined INSTALLED (echo [OK] Installed: !INSTALLED!) else (echo [FAIL] No compatible wheel found)
        )
    ) else (
        echo [WARN] Could not detect Python version - trying each wheel until one succeeds...
        set "INSTALLED="
        for %%f in ("%SCRIPT_DIR%python\*.whl") do (
            if not defined INSTALLED (
                echo Trying %%~nxf ...
                "!PYTHON_CMD!" -m pip install "%%f" --force-reinstall --no-deps --quiet 2>nul
                if !errorlevel! equ 0 set "INSTALLED=%%~nxf"
            )
        )
        if defined INSTALLED (echo [OK] Installed: !INSTALLED!) else (echo [FAIL] No compatible wheel found)
    )
) else (
    echo [SKIP] Python not found. Install Python 3.10+ and run:
    echo        pip install "%SCRIPT_DIR%python\omnihand-*.whl" --force-reinstall --no-deps
)
echo.

REM Add C:\omnihand\bin to system PATH
powershell -NoProfile -Command "$p = [Environment]::GetEnvironmentVariable('Path', 'Machine'); if ($p -notlike '*C:\omnihand\bin*') { [Environment]::SetEnvironmentVariable('Path', $p.TrimEnd(';') + ';C:\omnihand\bin', 'Machine'); Write-Host '[OK] Added C:\omnihand\bin to system PATH' } else { Write-Host '[OK] C:\omnihand\bin already in PATH' }"
echo.

if exist "%SCRIPT_DIR%ros2" (
    echo [INFO] ROS2 packages available at: %SCRIPT_DIR%ros2
)

echo ============================================
echo Installation Complete
echo ============================================
echo.
echo C++ SDK: %INSTALL_DIR%
echo PATH: C:\omnihand\bin has been added to system environment.
echo.
pause
endlocal
