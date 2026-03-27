#!/bin/bash
# OmniHand 2025 SDK - USB Permission Setup Script

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RULES_FILE="99-omnihand-usb.rules"
RULES_SRC="$SCRIPT_DIR/$RULES_FILE"
RULES_DST="/etc/udev/rules.d/$RULES_FILE"

# Check root for install
if [[ $EUID -ne 0 ]]; then
    echo "============================================"
    echo "OmniHand 2025 SDK - USB Permission Setup"
    echo "============================================"
    echo ""
    echo "Please run with sudo: sudo $0"
    exit 1
fi

echo "============================================"
echo "OmniHand 2025 SDK - USB Permission Setup"
echo "============================================"
echo ""

DETECTED_RULES=""
FOUND=0

# Detect USB devices with port info
echo "=== Detecting USB Devices ==="
for sysdev in /sys/bus/usb/devices/*; do
    [[ -d "$sysdev" ]] || continue
    [[ -f "$sysdev/idVendor" ]] || continue
    
    VID=$(cat "$sysdev/idVendor" 2>/dev/null)
    PID=$(cat "$sysdev/idProduct" 2>/dev/null)
    PRODUCT=$(cat "$sysdev/product" 2>/dev/null || echo "Unknown")
    KERNEL=$(basename "$sysdev")  # e.g., "3-6" or "3-6.1"
    
    # Skip root hubs and non-device entries
    [[ "$KERNEL" =~ ^[0-9]+-[0-9] ]] || continue
    
    # Check for known devices
    case "$VID:$PID" in
        3068:0009|3068:000a|3068:000b)  # ZLG USBCANFD series
            echo "  [FOUND] Port $KERNEL: ZLG USBCANFD - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# ZLG USBCANFD at port $KERNEL\n"
            DETECTED_RULES+="KERNEL==\"$KERNEL\", SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"$VID\", ATTRS{idProduct}==\"$PID\", MODE=\"0666\"\n\n"
            FOUND=$((FOUND + 1))
            ;;
        04da:0f01|04da:0f02)  # Another ZLG series
            echo "  [FOUND] Port $KERNEL: ZLG USBCAN - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# ZLG USBCAN at port $KERNEL\n"
            DETECTED_RULES+="KERNEL==\"$KERNEL\", SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"$VID\", ATTRS{idProduct}==\"$PID\", MODE=\"0666\"\n\n"
            FOUND=$((FOUND + 1))
            ;;
        0483:5740)  # STM32 Virtual COM Port
            echo "  [FOUND] Port $KERNEL: STM32 VCP - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# STM32 VCP at port $KERNEL\n"
            DETECTED_RULES+="KERNEL==\"$KERNEL\", SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"$VID\", ATTRS{idProduct}==\"$PID\", MODE=\"0666\"\n\n"
            FOUND=$((FOUND + 1))
            ;;
    esac
done

if [[ $FOUND -eq 0 ]]; then
    echo "  No known OmniHand/ZLG devices found."
fi
echo ""

# Also detect serial ports
echo "=== Detecting USB Serial Ports ==="
FOUND_SERIAL=0
for dev in /dev/ttyACM* /dev/ttyUSB*; do
    if [[ -e "$dev" ]]; then
        VID=$(udevadm info -a -n "$dev" 2>/dev/null | grep -m1 "ATTRS{idVendor}" | sed 's/.*=="\(.*\)"/\1/' || echo "")
        PID=$(udevadm info -a -n "$dev" 2>/dev/null | grep -m1 "ATTRS{idProduct}" | sed 's/.*=="\(.*\)"/\1/' || echo "")
        PRODUCT=$(udevadm info -a -n "$dev" 2>/dev/null | grep -m1 "ATTRS{product}" | sed 's/.*=="\(.*\)"/\1/' || echo "Unknown")
        KERNEL=$(udevadm info -a -n "$dev" 2>/dev/null | grep -m1 "KERNELS==" | grep -oE '[0-9]+-[0-9]+(\.[0-9]+)*' | head -1 || echo "")
        
        if [[ -n "$VID" && -n "$PID" ]]; then
            echo "  [FOUND] $dev (Port $KERNEL): $PRODUCT ($VID:$PID)"
            if [[ -n "$KERNEL" ]]; then
                DETECTED_RULES+="# $dev at port $KERNEL\n"
                DETECTED_RULES+="KERNEL==\"$KERNEL\", SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"$VID\", ATTRS{idProduct}==\"$PID\", MODE=\"0666\"\n\n"
            fi
            FOUND_SERIAL=$((FOUND_SERIAL + 1))
        fi
    fi
done

if [[ $FOUND_SERIAL -eq 0 ]]; then
    echo "  No USB serial ports found."
fi
echo ""

TOTAL=$((FOUND + FOUND_SERIAL))
echo "Total devices detected: $TOTAL"
echo ""

# Generate rules file
echo "=== Generating udev rules ==="
cat > "$RULES_SRC" << EOF
# OmniHand 2025 SDK - USB Device Permission Rules
# Generated on $(date)

# Generic rules for USB serial devices
KERNEL=="ttyACM[0-9]*", MODE="0666", GROUP="dialout"
KERNEL=="ttyUSB[0-9]*", MODE="0666", GROUP="dialout"

EOF

# Add detected devices
if [[ -n "$DETECTED_RULES" ]]; then
    echo "# Detected devices (with USB port):" >> "$RULES_SRC"
    echo -e "$DETECTED_RULES" >> "$RULES_SRC"
fi

echo "  Rules file: $RULES_SRC"
echo ""

# Install
echo "=== Installing ==="
cp "$RULES_SRC" "$RULES_DST"
echo "  Copied to: $RULES_DST"

# Add user to groups
CURRENT_USER="${SUDO_USER:-$USER}"
if getent group dialout > /dev/null 2>&1; then
    usermod -aG dialout "$CURRENT_USER" 2>/dev/null && echo "  Added '$CURRENT_USER' to 'dialout' group"
fi
if getent group plugdev > /dev/null 2>&1; then
    usermod -aG plugdev "$CURRENT_USER" 2>/dev/null && echo "  Added '$CURRENT_USER' to 'plugdev' group"
fi

# Reload
udevadm control --reload-rules
udevadm trigger
echo "  Reloaded udev rules"

echo ""
echo "============================================"
echo "Done! Detected $TOTAL device(s)."
echo "============================================"
echo ""
echo ">>> Please LOG OUT and LOG BACK IN <<<"
echo ""
