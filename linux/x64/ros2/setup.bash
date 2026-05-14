#!/bin/bash
# OmniHand ROS2 Setup - Auto-detect ROS distribution
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Supported ROS distributions (in order of preference)
for distro in humble iron jazzy rolling; do
    if [[ -d "$SCRIPT_DIR/$distro" ]] && [[ -f "$SCRIPT_DIR/$distro/setup.bash" ]]; then
        echo "[1/3] Checking ROS2 $distro installation..."
        if [[ ! -f "/opt/ros/$distro/setup.bash" ]]; then
            echo "Error: ROS2 $distro is not installed. Please install it first." >&2
            return 1
        fi
        echo "[2/3] Sourcing /opt/ros/$distro/setup.bash..."
        source "/opt/ros/$distro/setup.bash"
        if [[ $? -ne 0 ]]; then
            echo "Error: Failed to source /opt/ros/$distro/setup.bash" >&2
            return 1
        fi
        echo "[3/3] Sourcing $SCRIPT_DIR/$distro/setup.bash..."
        source "$SCRIPT_DIR/$distro/setup.bash"
        if [[ $? -eq 0 ]]; then
            echo "Success: Sourced $SCRIPT_DIR/$distro/setup.bash"
            return 0
        else
            echo "Warning: Failed to source $SCRIPT_DIR/$distro/setup.bash" >&2
        fi
    fi
done

echo "Error: No compatible ROS2 installation found in $SCRIPT_DIR" >&2
echo "Available directories:" >&2
ls -d "$SCRIPT_DIR"/*/ 2>/dev/null | sed 's|^|  |' >&2
return 1
