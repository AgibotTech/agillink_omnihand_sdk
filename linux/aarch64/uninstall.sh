#!/bin/bash
# OmniHand 2025 SDK Uninstall Script
set -e

PREFIX="${1:-/usr/local}"

echo "Uninstalling OmniHand 2025 SDK"
echo "  Install path: $PREFIX"
echo ""

# Uninstall C++ SDK
echo "Removing C++ SDK..."
sudo rm -rf "$PREFIX/include/omnihand"
sudo rm -f "$PREFIX/lib/libomnihand.so"*
sudo rm -f "$PREFIX/lib/libusbcanfd.so"*
sudo rm -f "$PREFIX/lib/libusb-1.0.so"*
sudo rm -f "$PREFIX/lib/libcanbus.so"*
sudo rm -f "$PREFIX/lib/libMarvinSDK.so"*
sudo rm -rf "$PREFIX/share/cmake/omnihand"
sudo ldconfig 2>/dev/null || true

# Uninstall Python package
echo "Removing Python package..."
# Try to uninstall from all Python versions that might have it installed
# Use python3 -m pip to ensure we use the same Python interpreter as installation
if command -v python3 >/dev/null 2>&1; then
    python3 -m pip uninstall -y omnihand 2>/dev/null || true
fi
# Also try common Python versions
for py_cmd in python3.10 python3.11 python3.12 python3.13; do
    if command -v "$py_cmd" >/dev/null 2>&1; then
        "$py_cmd" -m pip uninstall -y omnihand 2>/dev/null || true
    fi
done

echo ""
echo "Uninstallation complete!"
echo ""
echo "Note: ROS2 packages were not removed."
echo "      Remove them manually if needed."
