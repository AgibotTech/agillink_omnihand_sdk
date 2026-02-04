#!/bin/bash
# OmniHand ROS2 Setup - Auto-detect ROS distribution
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Supported ROS distributions (in order of preference)
for distro in humble iron jazzy rolling; do
    if [[ -d "$SCRIPT_DIR/$distro" ]] && [[ -f "$SCRIPT_DIR/$distro/setup.bash" ]]; then
        source "$SCRIPT_DIR/$distro/setup.bash"
        if [[ $? -eq 0 ]]; then
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
