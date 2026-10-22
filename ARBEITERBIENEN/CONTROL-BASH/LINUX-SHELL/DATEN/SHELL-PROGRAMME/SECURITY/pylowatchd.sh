#!/bin/bash

LOGDIR="/PYLOVARA-SYSTEM/MELDUNG/"
mkdir -p "$LOGDIR"

COUNT=0

log_entry() {
    COUNT=$((COUNT + 1))
    LOGFILE="$LOGDIR/log-$(printf "%05d" "$COUNT").pvlog"
    echo "==== LOG $COUNT ====" > "$LOGFILE"
    echo "Timestamp: $(date -u)" >> "$LOGFILE"
    echo "Current User: $(whoami)" >> "$LOGFILE"
    echo "Uptime: $(uptime)" >> "$LOGFILE"
    echo "--- suspicious su/sudo usage ---" >> "$LOGFILE"
    ps aux | grep -E 'sudo|su|strace|gdb|python|perl|nmap|bash' | grep -v grep >> "$LOGFILE"
    echo "--- Open connections ---" >> "$LOGFILE"
    ss -tpn >> "$LOGFILE"
    echo "--- Current Mounts ---" >> "$LOGFILE"
    mount | grep -E 'overlay|tmpfs|proc' >> "$LOGFILE"
}

while true; do
    sleep 15
    if pgrep -f 'sudo|su|LD_PRELOAD|gdb|strace|ncat' > /dev/null; then
        log_entry
    fi
done
