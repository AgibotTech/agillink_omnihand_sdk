#!/bin/bash
# AGILINK OmniHand SDK Install Script
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PREFIX="${1:-/usr/local}"

echo "Installing AGILINK OmniHand SDK"
echo "  Install path: $PREFIX"
echo ""

# Check if libraries exist
if [[ ! -d "$SCRIPT_DIR/cpp/lib" ]]; then
    echo "Error: Libraries not found!"
    echo "Please check the release package."
    exit 1
fi

# Install C++ SDK
echo "Installing C++ SDK..."

# Remove old version libraries first (avoid version conflicts)
sudo rm -f "$PREFIX/lib/libomnihand.so"* 2>/dev/null || true
sudo rm -f "$PREFIX/lib/libusbcanfd.so"* 2>/dev/null || true
sudo rm -f "$PREFIX/lib/libusb-1.0.so"* 2>/dev/null || true
sudo rm -f "$PREFIX/lib/libcanbus.so"* 2>/dev/null || true
sudo rm -f "$PREFIX/lib/libMarvinSDK.so"* 2>/dev/null || true

sudo mkdir -p "$PREFIX/include" "$PREFIX/lib" "$PREFIX/share/cmake/omnihand"
sudo cp -r "$SCRIPT_DIR/cpp/include/"* "$PREFIX/include/"
sudo cp -P "$SCRIPT_DIR/cpp/lib/"* "$PREFIX/lib/"
sudo cp "$SCRIPT_DIR/cpp/share/cmake/omnihand/"* "$PREFIX/share/cmake/omnihand/"
sudo ldconfig 2>/dev/null || true

# Install Python wheel (automatically select the correct one for current Python version)
if ls "$SCRIPT_DIR/python/"*.whl 1> /dev/null 2>&1; then
    echo "Installing Python package..."
    # Get current Python version and find matching wheel
    PY_VERSION=$(python3 -c "import sys; print(f'cp{sys.version_info.major}{sys.version_info.minor}')" 2>/dev/null || echo "")
    if [[ -n "$PY_VERSION" ]]; then
        MATCHING_WHL=$(ls "$SCRIPT_DIR/python/"*${PY_VERSION}*.whl 2>/dev/null | head -1)
        if [[ -n "$MATCHING_WHL" ]] && [[ -f "$MATCHING_WHL" ]]; then
            echo "  Found matching wheel for Python ${PY_VERSION}: $(basename "$MATCHING_WHL")"
            IS_SYSTEM=$(python3 -c "
import sys, os
exe = sys.executable
in_conda = 'CONDA_PREFIX' in os.environ and os.environ['CONDA_PREFIX'] != '' or 'conda' in exe
in_venv = 'VIRTUAL_ENV' in os.environ and os.environ['VIRTUAL_ENV'] != '' or sys.prefix != sys.base_prefix
is_system = exe.startswith('/usr/bin/') and not in_conda and not in_venv
print('true' if is_system else 'false')
")
            if [[ "$IS_SYSTEM" == "true" ]]; then
                echo " $(which python3) Installing system-wide..."
                if [[ "$PY_VERSION" =~ ^cp31[1-4]$ ]]; then
                    python3 -m pip install --force-reinstall "$MATCHING_WHL" --break-system-packages
                else
                    python3 -m pip install --force-reinstall "$MATCHING_WHL"
                fi
            else
                echo " $(which python3) Installing in user site-packages..."
                python3 -m pip install --force-reinstall "$MATCHING_WHL"
            fi
        else
            echo "  Warning: No matching wheel found for Python ${PY_VERSION}"
            echo "  Available wheels:"
            ls -1 "$SCRIPT_DIR/python/"*.whl 2>/dev/null | sed 's|^|    |' || true
            echo "  Attempting to install any compatible wheel..."
            python3 -m pip install --force-reinstall --find-links "$SCRIPT_DIR/python" omnihand || true
        fi
    else
        echo "  Warning: Could not determine Python version, attempting to install any compatible wheel..."
        python3 -m pip install --force-reinstall --find-links "$SCRIPT_DIR/python" omnihand || true
    fi
fi

# ROS2 (not auto-installed, prompt user to source manually)
if [[ -d "$SCRIPT_DIR/ros2" ]]; then
    echo ""
    echo "ROS2 packages (not auto-installed):"
    echo "  source $SCRIPT_DIR/ros2/setup.bash"
fi

echo ""
echo "Installation complete!"
echo ""
echo "C++ usage:"
echo "  list(APPEND CMAKE_MODULE_PATH \"$PREFIX/share/cmake/omnihand\")"
echo "  find_package(omnihand REQUIRED)"
