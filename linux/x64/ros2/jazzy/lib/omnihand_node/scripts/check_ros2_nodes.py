#!/usr/bin/env python3
"""
ROS2 Node Checker and Cleanup Tool
===================================
This script checks for active ROS2 nodes and provides options to kill them.

Usage:
    python3 check_ros2_nodes.py          # List all active nodes
    python3 check_ros2_nodes.py --kill   # Kill all omnihand nodes
    python3 check_ros2_nodes.py --kill-all # Kill all ROS2 nodes
"""

import sys
import subprocess
import signal
import os
import argparse


def run_command(cmd, check=False):
    """Run a shell command and return output."""
    try:
        result = subprocess.run(
            cmd, shell=True, capture_output=True, text=True, check=check
        )
        return result.stdout.strip(), result.returncode
    except Exception as e:
        return "", 1


def check_ros2_nodes():
    """Check active ROS2 nodes using ros2 command."""
    print("[1] Active ROS2 Nodes (via ros2 node list):")
    print("-" * 40)
    
    output, code = run_command("ros2 node list 2>/dev/null", check=False)
    if output:
        for line in output.split('\n'):
            if line.strip():
                print(f"  - {line.strip()}")
    else:
        print("  No active ROS2 nodes found (or ros2 command not available)")
    print()


def check_processes():
    """Check for related processes."""
    print("[2] Related Processes:")
    print("-" * 40)
    
    # Check for omnihand nodes
    output, _ = run_command("pgrep -f 'omnihand_2025.*node' 2>/dev/null", check=False)
    if output:
        pids = output.split()
        print(f"OmniHand Node Processes ({len(pids)}):")
        for pid in pids:
            ps_output, _ = run_command(f"ps -fp {pid} 2>/dev/null", check=False)
            if ps_output:
                lines = ps_output.split('\n')
                if len(lines) > 1:
                    print(f"  PID {pid}: {lines[-1]}")
        print()
    else:
        print("  No omnihand node processes found")
        print()
    
    # Check for Python ROS2 scripts
    output, _ = run_command("pgrep -f 'ros2.*run|rclpy|omnihand.*\\.py' 2>/dev/null", check=False)
    if output:
        pids = output.split()
        print(f"Python ROS2 Scripts ({len(pids)}):")
        for pid in pids:
            ps_output, _ = run_command(f"ps -fp {pid} 2>/dev/null", check=False)
            if ps_output:
                lines = ps_output.split('\n')
                if len(lines) > 1:
                    print(f"  PID {pid}: {lines[-1]}")
        print()
    
    # Check for ros2 daemon
    output, _ = run_command("pgrep -f 'ros2.*daemon' 2>/dev/null", check=False)
    if output:
        pid = output.split()[0]
        ps_output, _ = run_command(f"ps -fp {pid} 2>/dev/null", check=False)
        if ps_output:
            print("ROS2 Daemon:")
            print(f"  {ps_output.split(chr(10))[-1]}")
            print()


def check_canfd_devices():
    """Check CANFD device status."""
    print("[3] CANFD Device Status:")
    print("-" * 40)
    
    # Check USB devices
    output, _ = run_command("ls /dev/ttyUSB* 2>/dev/null", check=False)
    if output:
        devices = output.split()
        print(f"USB Devices ({len(devices)}):")
        for dev in devices:
            print(f"  - {dev}")
        print()
    
    # Check for usbcanfd processes
    output, _ = run_command("pgrep -f 'usbcanfd|zlgcan' 2>/dev/null", check=False)
    if output:
        pids = output.split()
        print(f"USB CANFD Related Processes ({len(pids)}):")
        for pid in pids:
            ps_output, _ = run_command(f"ps -fp {pid} 2>/dev/null", check=False)
            if ps_output:
                lines = ps_output.split('\n')
                if len(lines) > 1:
                    print(f"  PID {pid}: {lines[-1]}")
        print()
    else:
        print("  No USB CANFD processes found")
        print()


def kill_nodes(kill_all=False):
    """Kill ROS2 nodes."""
    if kill_all:
        print("[ACTION] Killing all ROS2 nodes...")
        run_command("pkill -f 'ros2.*run' 2>/dev/null", check=False)
        run_command("pkill -f 'rclpy' 2>/dev/null", check=False)
        run_command("pkill -f 'ros2.*daemon' 2>/dev/null", check=False)
        print("  All ROS2 processes killed")
    else:
        print("[ACTION] Killing omnihand nodes...")
        run_command("pkill -f 'omnihand_2025.*node' 2>/dev/null", check=False)
        run_command("pkill -f 'omnihand.*\\.py' 2>/dev/null", check=False)
        print("  OmniHand nodes killed")
    
    # Wait a bit
    import time
    time.sleep(1)
    
    # Force kill if still running
    if not kill_all:
        output, _ = run_command("pgrep -f 'omnihand_2025.*node|omnihand.*\\.py' 2>/dev/null", check=False)
        if output:
            print("[WARN] Some processes are still running, force killing...")
            run_command("pkill -9 -f 'omnihand_2025.*node' 2>/dev/null", check=False)
            run_command("pkill -9 -f 'omnihand.*\\.py' 2>/dev/null", check=False)
    
    print()
    print("Cleanup complete!")


def main():
    parser = argparse.ArgumentParser(
        description="Check and cleanup ROS2 nodes",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s              # List all active nodes
  %(prog)s --kill       # Kill all omnihand nodes
  %(prog)s --kill-all   # Kill all ROS2 nodes
        """
    )
    parser.add_argument(
        '--kill',
        action='store_true',
        help='Kill all omnihand nodes'
    )
    parser.add_argument(
        '--kill-all',
        action='store_true',
        help='Kill all ROS2 nodes'
    )
    
    args = parser.parse_args()
    
    print("=" * 40)
    print("ROS2 Node Checker and Cleanup")
    print("=" * 40)
    print()
    
    check_ros2_nodes()
    check_processes()
    check_canfd_devices()
    
    # Summary
    print("[4] Summary:")
    print("-" * 40)
    output, _ = run_command("pgrep -f 'omnihand_2025.*node' 2>/dev/null", check=False)
    omnihand_count = len(output.split()) if output else 0
    
    output, _ = run_command("pgrep -f 'omnihand.*\\.py' 2>/dev/null", check=False)
    python_count = len(output.split()) if output else 0
    
    print(f"  OmniHand node processes: {omnihand_count}")
    print(f"  Python ROS2 scripts: {python_count}")
    print()
    
    # Kill if requested
    if args.kill or args.kill_all:
        kill_nodes(kill_all=args.kill_all)
        print()
        print("Re-checking after cleanup...")
        print()
        check_ros2_nodes()
        check_processes()
    else:
        print("To kill omnihand nodes, run:")
        print(f"  {sys.argv[0]} --kill")
        print()
        print("To kill all ROS2 nodes, run:")
        print(f"  {sys.argv[0]} --kill-all")
    
    print("=" * 40)


if __name__ == '__main__':
    main()
