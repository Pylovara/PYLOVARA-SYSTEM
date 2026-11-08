#!/usr/bin/env bash
set -euo pipefail

# Konfiguration der Erfassung
STATUS_FILE=".mcs_status"
README_FILE="README.wiki-notes"

# Zähler initialisieren
mcs_modules=0
doc_files=0
total_files=0

# 1. Zählung der MCS-Inhalte (über Verzeichnisse und Kernkomponenten)
if [ -d "MCS-BIBLIOTHEK" ]; then
    mcs_modules=$(find MCS-BIBLIOTHEK -type f | wc -l)
fi

if [ -d "ARBEITERBIENEN" ]; then
    bee_modules=$(find ARBEITERBIENEN -type f | wc -l)
    mcs_modules=$((mcs_modules + bee_modules))
fi

# 2. Zählung von Dokumentationen / Snapshots / Handbüchern
if [ -d "HANDBÜCHER" ]; then
    doc_files=$(find HANDBÜCHER -type f | wc -l)
fi

if [ -d "SNAPSHOT-VERSIONEN" ]; then
    snap_files=$(find SNAPSHOT-VERSIONEN -type f | wc -l)
    doc_files=$((doc_files + snap_files))
fi

# Gesamtsumme ermitteln
total_files=$(find . -type f -not -path '*/.*' | wc -l)

if [ "$total_files" -eq 0 ]; then
    total_files=1
fi

# Prozentualen MCS-Anteil berechnen
mcs_percent=$(( (mcs_modules * 100) / total_files ))
doc_percent=$(( (doc_files * 100) / total_files ))
other_percent=$(( 100 - mcs_percent - doc_percent ))

# In die versteckte Datei schreiben (Werte für die visuelle Bindung)
cat << EOF > "$STATUS_FILE"
[TWS-STATUS: AKTIV]
MCS-MODULE: $mcs_modules
TOTAL-FILES: $total_files
MCS-ANTEIL: ${mcs_percent}%
DOC-ANTEIL: ${doc_percent}%
OTHER-ANTEIL: ${other_percent}%
TIMESTAMP: $(date -u '+%Y-%m-%d %H:%M:%S UTC')
EOF

echo "[✓] .mcs_status erfolgreich aktualisiert (${mcs_percent}% MCS-Substanz)."