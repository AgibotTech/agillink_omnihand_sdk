#!/bin/bash
# OmniHand ROS2 Setup - Auto-detect ROS distribution
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Supported ROS distributions (in order of preference)
for distro in humble iron jazzy rolling; do
    if [[ -d "$SCRIPT_DIR/$distro" ]] && [[ -f "$SCRIPT_DIR/$distro/setup.bash" ]]; then
        echo "[1/4] Checking ROS2 $distro installation..."
        if [[ ! -f "/opt/ros/$distro/setup.bash" ]]; then
            echo "Error: ROS2 $distro is not installed. Please install it first." >&2
            continue
        fi
        echo "[2/4] Sourcing /opt/ros/$distro/setup.bash..."
        source "/opt/ros/$distro/setup.bash"
        if [[ $? -ne 0 ]]; then
            echo "Error: Failed to source /opt/ros/$distro/setup.bash" >&2
            return 1
        fi
        echo "[3/4] Sourcing $SCRIPT_DIR/$distro/setup.bash..."
        source "$SCRIPT_DIR/$distro/setup.bash"
        if [[ $? -eq 0 ]]; then
            echo "[4/4] Checking bundled OmniHand runtime..."
            node_executable="$SCRIPT_DIR/$distro/lib/omnihand_node/omnihand_2025_node"
            if [[ ! -x "$node_executable" ]]; then
                echo "Error: ROS2 node executable is missing: $node_executable" >&2
                return 1
            fi
            if ! ldd_output=$(ldd "$node_executable" 2>&1); then
                echo "Error: Failed to inspect ROS2 node dependencies:" >&2
                echo "$ldd_output" >&2
                return 1
            fi
            missing_dependencies=$(echo "$ldd_output" | awk '/not found/{print $1}')
            if [[ -n "$missing_dependencies" ]]; then
                echo "Error: ROS2 node has unresolved runtime dependencies:" >&2
                echo "$missing_dependencies" | sed 's/^/  /' >&2
                echo "The bundled ROS2 runtime is incomplete." >&2
                return 1
            fi
            echo "Success: ROS2 $distro and bundled OmniHand runtime are ready"
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
