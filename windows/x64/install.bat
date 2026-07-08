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

REM Priority 1: Check if running inside an active conda/venv environment
if defined CONDA_PREFIX (
    if exist "!CONDA_PREFIX!\python.exe" (
        set "PYTHON_CMD=!CONDA_PREFIX!\python.exe"
        echo [INFO] Detected active Conda environment: !CONDA_PREFIX!
    )
)
if defined VIRTUAL_ENV (
    if not defined PYTHON_CMD (
        if exist "!VIRTUAL_ENV!\Scripts\python.exe" (
            set "PYTHON_CMD=!VIRTUAL_ENV!\Scripts\python.exe"
            echo [INFO] Detected active virtual environment: !VIRTUAL_ENV!
        )
    )
)

REM Priority 2: System Python via PATH (skip conda base python if not in active env)
if not defined PYTHON_CMD (
    for /f "delims=" %%p in ('where python 2^>nul') do (
        if not defined PYTHON_CMD (
            set "_PY_PATH=%%p"
            echo !_PY_PATH! | findstr /I /C:"conda" >nul 2>&1
            if !errorlevel! neq 0 (
                set "PYTHON_CMD=%%p"
            ) else (
                if defined CONDA_PREFIX (
                    set "PYTHON_CMD=%%p"
                )
            )
        )
    )
)

REM Priority 3: Well-known install locations
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python313\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python313\python.exe"
)
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python312\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python312\python.exe"
)
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python311\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python311\python.exe"
)
if not defined PYTHON_CMD (
    if exist "%LOCALAPPDATA%\Programs\Python\Python310\python.exe" set "PYTHON_CMD=%LOCALAPPDATA%\Programs\Python\Python310\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python313\python.exe" set "PYTHON_CMD=C:\Python313\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python312\python.exe" set "PYTHON_CMD=C:\Python312\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python311\python.exe" set "PYTHON_CMD=C:\Python311\python.exe"
)
if not defined PYTHON_CMD (
    if exist "C:\Python310\python.exe" set "PYTHON_CMD=C:\Python310\python.exe"
)

if defined PYTHON_CMD (
    echo Using Python: !PYTHON_CMD!

    REM Detect environment type (conda/venv/system) for pip args
    set "PIP_EXTRA_ARGS="
    for /f "delims=" %%r in ('"!PYTHON_CMD!" -c "import sys,os; in_conda=('CONDA_PREFIX' in os.environ and os.environ['CONDA_PREFIX']!='') or 'conda' in sys.executable.lower(); in_venv=('VIRTUAL_ENV' in os.environ and os.environ['VIRTUAL_ENV']!='') or sys.prefix!=sys.base_prefix; print('conda' if in_conda else ('venv' if in_venv else 'system'))" 2^>nul') do set "PY_ENV_TYPE=%%r"
    if "!PY_ENV_TYPE!"=="system" (
        echo [INFO] System Python detected - installing with --user
        set "PIP_EXTRA_ARGS=--user"
    ) else (
        echo [INFO] !PY_ENV_TYPE! Python detected - installing to environment
    )

    set "PY_VER="
    for /f "delims=" %%v in ('"!PYTHON_CMD!" -c "import sys; print(str(sys.version_info.major)+str(sys.version_info.minor))" 2^>nul') do set "PY_VER=%%v"
    if defined PY_VER (
        set "WHL_MATCH="
        for %%f in ("%SCRIPT_DIR%python\*-cp!PY_VER!-cp!PY_VER!-*.whl") do (
            set "WHL_MATCH=1"
            echo Installing %%~nxf ^(Python !PY_VER!^) ...
            "!PYTHON_CMD!" -m pip install "%%f" --force-reinstall --no-deps !PIP_EXTRA_ARGS! --quiet
            if !errorlevel! equ 0 (echo [OK] Installed: %%~nxf) else (echo [FAIL] %%~nxf)
        )
        if defined WHL_MATCH (
            echo [OK] Python package installed for Python !PY_VER!
        ) else (
            echo [WARN] No wheel for Python !PY_VER! - trying all wheels...
            set "INSTALLED="
            for %%f in ("%SCRIPT_DIR%python\*.whl") do (
                "!PYTHON_CMD!" -m pip install "%%f" --force-reinstall --no-deps !PIP_EXTRA_ARGS! --quiet 2>nul
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
                "!PYTHON_CMD!" -m pip install "%%f" --force-reinstall --no-deps !PIP_EXTRA_ARGS! --quiet 2>nul
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
