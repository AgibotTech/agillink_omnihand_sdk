#!/bin/bash
# AGILINK OmniHand SDK - USB Permission Setup Script

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RULES_FILE="99-omnihand-usb.rules"
RULES_SRC="$SCRIPT_DIR/$RULES_FILE"
RULES_DST="/etc/udev/rules.d/$RULES_FILE"

# Check root for install
if [[ $EUID -ne 0 ]]; then
    echo "============================================"
    echo "AGILINK OmniHand SDK - USB Permission Setup"
    echo "============================================"
    echo ""
    echo "Please run with sudo: sudo $0"
    exit 1
fi

echo "============================================"
echo "AGILINK OmniHand SDK - USB Permission Setup"
echo "============================================"
echo ""

DETECTED_RULES=""
FOUND=0

function add_rule() {
    local VID="$1"
    local PID="$2"
    local SERIAL="$3"
    if [[ -n "$SERIAL" ]]; then
        DETECTED_RULES+="SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"$VID\", ATTRS{idProduct}==\"$PID\", ATTRS{serial}==\"$SERIAL\", MODE=\"0666\"\n\n"
    fi
}

# Detect USB devices with port info
echo "=== Detecting USB Devices ==="
for sysdev in /sys/bus/usb/devices/*; do
    [[ -d "$sysdev" ]] || continue
    [[ -f "$sysdev/idVendor" ]] || continue
    
    VID=$(cat "$sysdev/idVendor" 2>/dev/null)
    PID=$(cat "$sysdev/idProduct" 2>/dev/null)
    PRODUCT=$(cat "$sysdev/product" 2>/dev/null || echo "Unknown")
    SERIAL=$(cat "$sysdev/serial" 2>/dev/null || echo "")
    
    
    # Check for known devices
    case "$VID:$PID" in
        3068:0009|3068:000a|3068:000b)  # ZLG USBCANFD series
            echo "  [FOUND] Port Serial $SERIAL: ZLG USBCANFD - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# ZLG USBCANFD Serial $SERIAL\n"
            add_rule "$VID" "$PID" "$SERIAL"
            FOUND=$((FOUND + 1))
            ;;
        04da:0f01|04da:0f02)  # Another ZLG series
            echo "  [FOUND] Port Serial $SERIAL: ZLG USBCAN - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# ZLG USBCAN Serial $SERIAL\n"
            add_rule "$VID" "$PID" "$SERIAL"
            FOUND=$((FOUND + 1))
            ;;
        a8fa:8598)  # HCAN USBCAN device
            echo "  [FOUND] Port Serial $SERIAL: HCAN USBCAN - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# HCAN USBCAN Serial $SERIAL\n"
            add_rule "$VID" "$PID" "$SERIAL"
            FOUND=$((FOUND + 1))
            ;;
        0483:5740)  # STM32 Virtual COM Port
            echo "  [FOUND] Port Serial $SERIAL: STM32 VCP - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# STM32 VCP Serial $SERIAL\n"
            add_rule "$VID" "$PID" "$SERIAL"
            FOUND=$((FOUND + 1))
            ;;
        3325:0049)  # MANUS Glove device
            echo "  [FOUND] Port Serial $SERIAL: MANUS Glove - $PRODUCT ($VID:$PID)"
            DETECTED_RULES+="# MANUS Glove Serial $SERIAL\n"
            add_rule "$VID" "$PID" "$SERIAL"
            FOUND=$((FOUND + 1))
            ;;
    esac
done

if [[ $FOUND -eq 0 ]]; then
    echo "  No known OmniHand/ZLG devices found."
fi
echo ""


TOTAL=$((FOUND ))
echo "Total devices detected: $TOTAL"
echo ""

# Generate rules file
echo "=== Generating udev rules ==="
cat > "$RULES_SRC" << EOF
# AGILINK OmniHand SDK - USB Device Permission Rules
# Generated on $(date)

# Generic rules for USB serial devices
KERNEL=="ttyACM[0-9]*", MODE="0666", GROUP="dialout"
KERNEL=="ttyUSB[0-9]*", MODE="0666", GROUP="dialout"
KERNEL=="ttySWK[0-9]*", MODE="0666", GROUP="dialout"

# Generic rule for CAN devices
SUBSYSTEM=="usb", ATTRS{idVendor}=="3068", ATTRS{idProduct}=="0009", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="3068", ATTRS{idProduct}=="000a", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="3068", ATTRS{idProduct}=="000b", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="04da", ATTRS{idProduct}=="0f01", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="04da", ATTRS{idProduct}=="0f02", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="a8fa", ATTRS{idProduct}=="8598", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="3325", ATTRS{idProduct}=="0049", MODE="0666"

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
