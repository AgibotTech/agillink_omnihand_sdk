#!/usr/bin/env bash
#
# 检查 ZLG USBCANFD 当前处于「内核 SocketCAN(usbcanfd)」还是「用户态 ZLG 库」路径，
# 并在两种模式间切换。二者互斥：加载 usbcanfd 后走 can0/candump；卸载后 USB 设备由 ZLG 库占用。
#
# 用法:
#   ./switch_zlgcan_mode.sh status
#   sudo ./switch_zlgcan_mode.sh socketcan    # 加载内核驱动 + driver_load.sh + 1M/5M 位时序
#   sudo ./switch_zlgcan_mode.sh zlg          # 卸载 usbcanfd，恢复 ZLG 用户态库
#
# 依赖: cpp/third_party/zlgcan/linux/socketcan 下 Makefile；若无 usbcanfd.ko，socketcan 子命令会自动 make。
#
# SocketCAN 模式下位时序与 SDK ZLG 用户态路径一致（见 zlg_usbcanfd_device.cc / SOCKETCAN_SETUP.md）：
#   仲裁段 1 Mbps @ 80%，数据段 5 Mbps @ 75%
#
# can_dev 与 usbcanfd：
#   - can_dev 是内核 CAN 设备公共层；usbcanfd（ZLG USB 驱动）依赖它，故加载时会 modprobe can_dev。
#   - 切回 ZLG 用户态只需 rmmod usbcanfd，让 USB 设备脱离内核驱动即可；不必卸 can_dev。
#   - can_dev 可能被本机其它 CAN 控制器共用；贸然 rmmod can_dev 可能失败或影响其它设备。

set -u

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
readonly DRIVER_DIR="${REPO_ROOT}/cpp/third_party/zlgcan/linux/socketcan"
readonly DRIVER_LOAD="${DRIVER_DIR}/driver_load.sh"
readonly KO_FILE="${DRIVER_DIR}/usbcanfd.ko"
readonly DRIVER_MAKEFILE="${DRIVER_DIR}/Makefile"

# OmniHand CAN FD：与文档 `ip link set ... sample-point 0.8 dsample-point 0.75` 一致
readonly OH_CAN_BITRATE=1000000
readonly OH_CAN_DBITRATE=5000000
readonly OH_SAMPLE_POINT='0.8'
readonly OH_DSAMPLE_POINT='0.75'

# 常见 ZLG USBCANFD 的 USB ID（readme.txt）；用于 status 提示
readonly ZLG_USB_PATTERNS='3068:0009|04cc:1240'

usage() {
  cat <<'EOF'
用法:
  ./switch_zlgcan_mode.sh status              # 查看模块 / can 接口 / USB
  sudo ./switch_zlgcan_mode.sh socketcan      # 加载 usbcanfd + driver_load.sh + OmniHand 位时序
  sudo ./switch_zlgcan_mode.sh zlgcan            # 卸载 usbcanfd，便于 ZLG 用户态库
别名: check | socketcan|sc|kernel | zlgcan|zlg|userspace|lib
EOF
}

need_root() {
  if [[ "${EUID:-$(id -u)}" -ne 0 ]]; then
    echo "[错误] 该操作需要 root，请使用: sudo $0 $*" >&2
    exit 1
  fi
}

# 若不存在 usbcanfd.ko，则在驱动目录执行 make（需已安装内核头文件与编译器）
ensure_usbcanfd_ko() {
  if [[ -f "${KO_FILE}" ]]; then
    return 0
  fi

  echo "未找到 usbcanfd.ko，正在编译: ${DRIVER_DIR}"
  if [[ ! -f "${DRIVER_MAKEFILE}" ]]; then
    echo "[错误] 无 Makefile: ${DRIVER_MAKEFILE}" >&2
    exit 1
  fi

  if ! command -v make >/dev/null 2>&1; then
    echo "[错误] 未找到 make，请先: sudo apt-get install -y build-essential" >&2
    exit 1
  fi

  if ! (cd "${DRIVER_DIR}" && make); then
    echo "[错误] 编译失败。常见依赖:" >&2
    echo "  sudo apt-get install -y build-essential gcc-12 linux-headers-\$(uname -r)" >&2
    exit 1
  fi

  if [[ ! -f "${KO_FILE}" ]]; then
    echo "[错误] make 已完成但仍未生成 ${KO_FILE}，请查看上方编译日志" >&2
    exit 1
  fi
  echo "已生成 ${KO_FILE}"
}

kill_can_utils() {
  for name in cangen candump cansend; do
    pids=$(pgrep -f "^${name}" 2>/dev/null || true)
    if [[ -n "${pids}" ]]; then
      echo "结束进程: ${name}"
      kill ${pids} 2>/dev/null || true
    fi
  done
}

bring_down_can_ifaces() {
  local devs
  devs=$(ip -o link show 2>/dev/null | awk -F': ' '/^[0-9]+: can/ {print $2}' || true)
  for d in ${devs}; do
    echo "关闭接口: ${d}"
    ip link set "${d}" down 2>/dev/null || true
  done
}

# 枚举 sysfs 下 can0、can1…（不含 lin*）；用于配置位时序
list_socketcan_ifaces() {
  local p name
  for p in /sys/class/net/can[0-9]*; do
    [[ -e "${p}" ]] || continue
    name="$(basename "${p}")"
    [[ "${name}" =~ ^can[0-9]+$ ]] || continue
    echo "${name}"
  done | sort -V
}

# driver_load.sh 可能写成 500k/2M；此处覆盖为 OmniHand 与 SDK 一致的 1M/5M @ 80%/75%
apply_omnihand_socketcan_timing() {
  local iface
  if ! command -v ip >/dev/null 2>&1; then
    echo "[错误] 未找到 ip 命令，无法配置 CAN 位时序" >&2
    return 1
  fi

  echo "配置 CAN FD: ${OH_CAN_BITRATE}/${OH_CAN_DBITRATE}, sample ${OH_SAMPLE_POINT}/${OH_DSAMPLE_POINT}"

  if [[ -z "$(list_socketcan_ifaces)" ]]; then
    echo "未发现 can 接口" >&2
    return 0
  fi

  while IFS= read -r iface; do
    [[ -n "${iface}" ]] || continue
    echo "  ${iface}"
    ip link set "${iface}" down 2>/dev/null || true
    if ! ip link set "${iface}" type can fd on \
        bitrate "${OH_CAN_BITRATE}" dbitrate "${OH_CAN_DBITRATE}" \
        sample-point "${OH_SAMPLE_POINT}" dsample-point "${OH_DSAMPLE_POINT}"; then
      echo "${iface}: 位时序设置失败，跳过" >&2
      continue
    fi
    ip link set "${iface}" up || echo "${iface}: up 失败" >&2
  done < <(list_socketcan_ifaces)

  while IFS= read -r iface; do
    [[ -n "${iface}" ]] || continue
    ip -details link show "${iface}" 2>/dev/null | head -n 2 || true
  done < <(list_socketcan_ifaces)
}

cmd_status() {
  echo "模块:"
  if lsmod | grep -q '^usbcanfd'; then
    echo "  usbcanfd 已加载 (insmod 自 usbcanfd.ko)"
  else
    echo "  usbcanfd 未加载"
  fi
  lsmod | grep -q '^can_dev' && echo "  can_dev 已加载" || echo "  can_dev 未加载"

  echo "接口:"
  if command -v ip >/dev/null 2>&1; then
    ip -br link show 2>/dev/null | grep -E '^can[0-9]' || echo "  (无)"
  else
    echo "  (无 ip)"
  fi

  echo "USB:"
  if command -v lsusb >/dev/null 2>&1; then
    lsusb 2>/dev/null | grep -E "${ZLG_USB_PATTERNS}" || echo "  (无匹配 ${ZLG_USB_PATTERNS})"
  else
    echo "  (无 lsusb)"
  fi

  echo "文件:"
  [[ -f "${DRIVER_LOAD}" ]] && echo "  driver_load.sh OK" || echo "  缺 driver_load.sh"
  [[ -f "${KO_FILE}" ]] && echo "  usbcanfd.ko OK" || echo "  缺 usbcanfd.ko (socketcan 时会自动 make)"
}

cmd_socketcan() {
  need_root socketcan

  if [[ ! -f "${DRIVER_LOAD}" ]]; then
    echo "[错误] 找不到 ${DRIVER_LOAD}" >&2
    exit 1
  fi

  ensure_usbcanfd_ko

  echo "driver_load.sh ..."
  (cd "${DRIVER_DIR}" && bash "${DRIVER_LOAD}")

  apply_omnihand_socketcan_timing

  echo "完成。${0} status"
}

cmd_zlg() {
  need_root zlg

  kill_can_utils
  bring_down_can_ifaces

  # 卸载内核模块 usbcanfd（由 usbcanfd.ko insmod 得到），与 rmmod usbcanfd.ko 等价
  if lsmod | grep -q '^usbcanfd'; then
    echo "rmmod usbcanfd (内核模块名；对应 usbcanfd.ko)"
    modprobe -r usbcanfd 2>/dev/null || rmmod usbcanfd 2>/dev/null || true
  else
    echo "usbcanfd 未加载"
  fi

  # 不 rmmod can_dev：见文件头说明；需要时可手动 modprobe -r can_dev（无其它 CAN 时）
  if lsmod | grep -q '^can_dev'; then
    echo "can_dev 仍加载（正常；与 usbcanfd 无关）"
  fi

  echo "完成。${0} status"
}

main() {
  local sub="${1:-status}"
  case "${sub}" in
    status|check|st)
      cmd_status
      ;;
    socketcan|sc|kernel)
      cmd_socketcan
      ;;
    zlgcan|zlg|userspace|lib)
      cmd_zlg
      ;;
    -h|--help|help)
      usage
      ;;
    *)
      echo "未知子命令: ${sub}" >&2
      usage >&2
      exit 1
      ;;
  esac
}

main "$@"
