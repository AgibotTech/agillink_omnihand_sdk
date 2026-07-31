#!/bin/bash
# AGILINK OmniHand SDK Uninstall Script
set -e

PREFIX="${1:-/usr/local}"

echo "Uninstalling AGILINK OmniHand SDK"
echo "  Install path: $PREFIX"
echo ""

# Uninstall C++ SDK
echo "Removing C++ SDK..."
sudo rm -rf "$PREFIX/include/omnihand"
sudo rm -f "$PREFIX/lib/libomnihand.so"*
sudo rm -f "$PREFIX/lib/libusbcanfd.so"*
sudo rm -f "$PREFIX/lib/libusb-1.0.so"*
sudo rm -f "$PREFIX/lib/libcanbus.so"*
sudo rm -rf "$PREFIX/share/cmake/omnihand"
sudo ldconfig 2>/dev/null || true

# Uninstall Python package
echo "Removing Python package..."
if command -v python3 >/dev/null 2>&1; then
    PY_VERSION=$(python3 -c "import sys; print(f'cp{sys.version_info.major}{sys.version_info.minor}')" 2>/dev/null || echo "")
    IS_SYSTEM=$(python3 -c "
import sys, os
exe = sys.executable
in_conda = 'CONDA_PREFIX' in os.environ and os.environ['CONDA_PREFIX'] != '' or 'conda' in exe
in_venv = 'VIRTUAL_ENV' in os.environ and os.environ['VIRTUAL_ENV'] != '' or sys.prefix != sys.base_prefix
is_system = exe.startswith('/usr/bin/') and not in_conda and not in_venv
print('true' if is_system else 'false')
")
    if [[ "$IS_SYSTEM" == "true" ]]; then
        echo " $(which python3) Uninstalling system-wide..."
        if [[ "$PY_VERSION" =~ ^cp31[1-4]$ ]]; then
            python3 -m pip uninstall -y omnihand --break-system-packages 2>/dev/null || true
        else
            python3 -m pip uninstall -y omnihand 2>/dev/null || true
        fi
    else
        echo " $(which python3) Uninstalling in user site-packages..."
        python3 -m pip uninstall -y omnihand 2>/dev/null || true
    fi
fi
for py_cmd in python3.10 python3.11 python3.12 python3.13 python3.14; do
        "$py_cmd" -m pip uninstall -y omnihand 2>/dev/null || true
done

echo ""
echo "Uninstallation complete!"
echo ""
echo "Note: ROS2 packages were not removed."
echo "      Remove them manually if needed."
