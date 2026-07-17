#!/usr/bin/env bash
#
# setup_socketcan.sh --- OmniHand SocketCAN bring-up helper.
#
# Enumerates every canN under /sys/class/net and applies the OmniHand CAN FD
# bit timings from release/doc/en/SOCKETCAN_SETUP.md section 3:
#
#   arbitration 1 Mbps @ sample-point 0.8
#   data        5 Mbps @ dsample-point 0.75
#
# Self-contained: no repo-relative paths, safe to ship in the release tarball
# next to install.sh. Requires root (or CAP_NET_ADMIN on `ip`).
#
# Usage:
#   sudo ./setup_socketcan.sh                 # apply to every canN
#   sudo ./setup_socketcan.sh can0 can3       # only the listed ones
#        ./setup_socketcan.sh --dry-run       # print commands, no changes
#        ./setup_socketcan.sh -h | --help
#
# Exit status:
#   0   success (or nothing to do)
#   1   user / environment error (no `ip`, not root, bad flag)
#   2   at least one interface failed to configure

set -u

# OmniHand CAN FD defaults — keep in sync with:
#   release/doc/en/SOCKETCAN_SETUP.md           (section 3)
#   cpp/src/can/linux_socket_can_impl.cc        (auto-configure path)
#   scripts/switch_zlgcan_mode.sh               (apply_omnihand_socketcan_timing)
readonly OH_BITRATE=1000000
readonly OH_DBITRATE=5000000
readonly OH_SAMPLE_POINT='0.8'
readonly OH_DSAMPLE_POINT='0.75'

usage() {
  cat <<'EOF'
Usage:
  sudo ./setup_socketcan.sh              Apply to every canN under /sys/class/net
  sudo ./setup_socketcan.sh can0 can3    Apply only to the listed interfaces
       ./setup_socketcan.sh --dry-run    Print commands without executing
  sudo ./setup_socketcan.sh --install    Install as a systemd service + udev rule (persistent across reboots and interface resets)
  sudo ./setup_socketcan.sh --uninstall  Remove the systemd service and udev rule
       ./setup_socketcan.sh -h|--help    This message

OmniHand CAN FD timings applied:
  arbitration 1 Mbps @ sample-point 0.8
  data        5 Mbps @ dsample-point 0.75

See release/doc/en/SOCKETCAN_SETUP.md for driver install / verification /
teardown steps.
EOF
}

log()  { echo "[setup_socketcan] $*"; }
warn() { echo "[setup_socketcan][WARN ] $*" >&2; }
err()  { echo "[setup_socketcan][ERROR] $*" >&2; }

# Enumerate real canN interfaces (canfdN etc. are rejected).
list_can_ifaces() {
  local p name
  for p in /sys/class/net/can[0-9]*; do
    [[ -e "${p}" ]] || continue
    name="$(basename "${p}")"
    [[ "${name}" =~ ^can[0-9]+$ ]] || continue
    echo "${name}"
  done | sort -V
}

DRY_RUN=0
declare -a CLI_IFACES=()

while (( $# > 0 )); do
  case "$1" in
    -h|--help)  usage; exit 0 ;;
    --dry-run)  DRY_RUN=1; shift ;;
    --install)
      if [[ "${EUID:-$(id -u)}" -ne 0 ]]; then
        err "root required for --install. Rerun as: sudo $0 --install"
        exit 1
      fi
      INSTALL_PATH=/usr/local/bin/omnihand-setup-socketcan
      SERVICE=/etc/systemd/system/omnihand-socketcan.service
      UDEV_RULE=/etc/udev/rules.d/80-omnihand-socketcan.rules
      install -m 755 "$(cd "$(dirname "$0")" && pwd)/$(basename "$0")" "${INSTALL_PATH}"
      cat > "${SERVICE}" <<'UNIT'
[Unit]
Description=OmniHand SocketCAN interface setup (1M@80% / 5M@75%)
After=systemd-modules-load.service
Before=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/omnihand-setup-socketcan
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
UNIT
      cat > "${UDEV_RULE}" <<'UDEV'
SUBSYSTEM=="net", ACTION=="add", KERNEL=="can[0-9]*", RUN+="/usr/local/bin/omnihand-setup-socketcan %k"
UDEV
      systemctl daemon-reload
      systemctl enable omnihand-socketcan.service
      udevadm control --reload-rules
      udevadm trigger --action=add --subsystem-match=net
      log "script installed:  ${INSTALL_PATH}"
      log "service installed: ${SERVICE}"
      log "udev rule:         ${UDEV_RULE}"
      log "enabled:   sudo systemctl start omnihand-socketcan   (or reboot)"
      exit 0
      ;;
    --uninstall)
      if [[ "${EUID:-$(id -u)}" -ne 0 ]]; then
        err "root required for --uninstall. Rerun as: sudo $0 --uninstall"
        exit 1
      fi
      systemctl disable --now omnihand-socketcan.service 2>/dev/null || true
      rm -f /etc/systemd/system/omnihand-socketcan.service
      rm -f /usr/local/bin/omnihand-setup-socketcan
      rm -f /etc/udev/rules.d/80-omnihand-socketcan.rules
      systemctl daemon-reload
      udevadm control --reload-rules
      log "omnihand-socketcan service and udev rule removed"
      exit 0
      ;;
    --)         shift; while (( $# > 0 )); do CLI_IFACES+=("$1"); shift; done ;;
    -*)         err "unknown flag: $1"; usage >&2; exit 1 ;;
    *)          CLI_IFACES+=("$1"); shift ;;
  esac
done

if ! command -v ip >/dev/null 2>&1; then
  err "'ip' command not found; install iproute2 first"
  exit 1
fi

if (( DRY_RUN == 0 )) && [[ "${EUID:-$(id -u)}" -ne 0 ]]; then
  err "root required (ip link needs CAP_NET_ADMIN). Rerun as: sudo $0 $*"
  exit 1
fi

# Build the final iface list: explicit CLI args (validated) or auto-enumerate.
declare -a IFACES=()
if (( ${#CLI_IFACES[@]} > 0 )); then
  for iface in "${CLI_IFACES[@]}"; do
    if [[ ! -e "/sys/class/net/${iface}" ]]; then
      warn "${iface}: not present under /sys/class/net, skipping"
      continue
    fi
    IFACES+=("${iface}")
  done
else
  while IFS= read -r iface; do
    [[ -n "${iface}" ]] && IFACES+=("${iface}")
  done < <(list_can_ifaces)
fi

if (( ${#IFACES[@]} == 0 )); then
  log "no can interfaces found (nothing to do)"
  log "hint: 'ip link show' should list canN; if not, see SOCKETCAN_SETUP.md sections 1-2"
  exit 0
fi

log "targets: ${IFACES[*]}"
log "timings: bitrate=${OH_BITRATE} sp=${OH_SAMPLE_POINT}, dbitrate=${OH_DBITRATE} dsp=${OH_DSAMPLE_POINT} (fd on)"

# `run` prints the command in dry-run mode, executes it otherwise.
run() {
  if (( DRY_RUN )); then
    echo "+ $*"
  else
    "$@"
  fi
}

FAIL=0
for iface in "${IFACES[@]}"; do
  log "-> ${iface}"

  if ! run ip link set "${iface}" down; then
    warn "${iface}: 'ip link set ${iface} down' failed"
    FAIL=1
    continue
  fi

  if ! run ip link set "${iface}" type can fd on \
         bitrate    "${OH_BITRATE}"    sample-point  "${OH_SAMPLE_POINT}" \
         dbitrate   "${OH_DBITRATE}"   dsample-point "${OH_DSAMPLE_POINT}"; then
    warn "${iface}: 'ip link set ${iface} type can fd on ...' failed; iface left DOWN"
    FAIL=1
    continue
  fi

  if ! run ip link set "${iface}" up; then
    warn "${iface}: 'ip link set ${iface} up' failed"
    FAIL=1
    continue
  fi
done

if (( DRY_RUN == 0 )); then
  log "verify (ip -details link show):"
  for iface in "${IFACES[@]}"; do
    ip -details link show "${iface}" 2>/dev/null || true
    echo
  done
fi

if (( FAIL )); then
  err "one or more interfaces failed to configure (see warnings above)"
  exit 2
fi

log "done"
