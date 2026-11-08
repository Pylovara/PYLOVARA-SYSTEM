#!/usr/bin/env bash
# =============================================================================
#  PYLOVARA-SYSTEM // MCS ANALYZER 2.0
#  Erkennt MCS-Code inhaltlich am Transaktionsrahmen (kein Datei-Prefix nötig).
#  Erzeugt: .mcs_status, MCS-STATUS.md, MCS-STATUS.svg, README-Block
# =============================================================================
set -euo pipefail

# -----------------------------------------------------------------------------
#  1) MCS-SYNTAX — nur hier anpassen, wenn sich MCS syntaktisch ändert
# -----------------------------------------------------------------------------
MCS_OPEN="${MCS_OPEN:-¢!}"                 # Anfang Transaktionsrahmen
MCS_CLOSE="${MCS_CLOSE:-!¢}"               # Ende Transaktionsrahmen
MCS_DELIM_MODE="${MCS_DELIM_MODE:-line}"   # line = Token allein in Zeile; contains = Token irgendwo in Zeile
MCS_ALLOW_TRAILING_COMMENT="${MCS_ALLOW_TRAILING_COMMENT:-0}"
MCS_NESTING="${MCS_NESTING:-1}"            # verschachtelte Rahmen erlauben
MCS_DOMINANT_THRESHOLD="${MCS_DOMINANT_THRESHOLD:-80}"  # Prozent für "MCS-dominant"
MCS_COUNT_UNMATCHED="${MCS_COUNT_UNMATCHED:-0}"         # 0 = nur vollständige Paare
MCS_IGNORE_MARKER_BLOCKS="${MCS_IGNORE_MARKER_BLOCKS:-1}" # generierte README-Blöcke nicht mitzählen

# -----------------------------------------------------------------------------
#  2) DATEI- UND SPRACHERKENNUNG
#     Format: "endung|anzeigename|kategorie"
#     kategorie: code=Fremdcode, doc=Doku, config=Konfig, data=Daten,
#                binary=Binärmedium, mcs=MCS, unknown=Unbekannt
# -----------------------------------------------------------------------------
LANG_RULES=(
  "mcs|MCS|mcs"
  "sh|Bash|code" "bash|Bash|code" "zsh|Zsh|code" "fish|Fish|code"
  "ksh|Korn-Shell|code" "dash|Dash|code" "awk|AWK|code" "sed|Sed|code"
  "c|C|code" "h|C/C++-Header|code" "cpp|C++|code" "cc|C++|code"
  "cxx|C++|code" "hpp|C++-Header|code" "hxx|C++-Header|code"
  "rs|Rust|code" "py|Python|code" "pyw|Python|code" "pyi|Python|code"
  "kt|Kotlin|code" "kts|Kotlin|code" "java|Java|code"
  "php|PHP|code" "phtml|PHP|code"
  "js|JavaScript|code" "mjs|JavaScript|code" "cjs|JavaScript|code"
  "jsx|JavaScript|code" "ts|TypeScript|code" "tsx|TypeScript|code"
  "go|Go|code" "rb|Ruby|code" "pl|Perl|code" "pm|Perl|code"
  "lua|Lua|code" "cs|C#|code" "swift|Swift|code" "scala|Scala|code"
  "dart|Dart|code" "r|R|code" "jl|Julia|code" "hs|Haskell|code"
  "clj|Clojure|code" "cljs|ClojureScript|code" "ex|Elixir|code"
  "exs|Elixir|code" "erl|Erlang|code" "fs|F#|code" "vb|Visual Basic|code"
  "pas|Pascal|code" "pp|Pascal|code" "asm|Assembler|code" "s|Assembler|code"
  "sql|SQL|code" "graphql|GraphQL|code" "gql|GraphQL|code"
  "vue|Vue|code" "svelte|Svelte|code" "astro|Astro|code"
  "html|HTML|code" "htm|HTML|code" "xhtml|XHTML|code"
  "css|CSS|code" "scss|SCSS|code" "sass|Sass|code" "less|Less|code"
  "ps1|PowerShell|code" "psm1|PowerShell|code" "bat|Batch|code" "cmd|Batch|code"
  "v|Verilog/V|code" "vhd|VHDL|code" "vhdl|VHDL|code" "m|Objective-C/MATLAB|code"
  # Pylovara-eigene Bash-/DNA-Endungen
  "fullinstall-core|Bash|code" "install-core|Bash|code" "updater-core|Bash|code"
  "wrapper-shell|Bash|code" "source-bridge-core|Bash|code" "pylovara-core|Bash|code"
  "source|Bash|code" "dna-cmd|Bash|code" "cut-dna|Bash|code" "shell-dna|Bash|code"
  "trigger-dna|Bash|code" "record-dna|Bash|code" "to-dna|Bash|code" "tree-dna|Bash|code"
  "completion-dna|Bash|code" "cmd-dna|Bash|code" "basename-dna|Bash|code"
  "shell-layout|Bash|code" "ini-parser-logic|Bash|code"
  # Doku / Notizen / Lizenzen
  "md|Markdown|doc" "markdown|Markdown|doc" "txt|Text|doc" "text|Text|doc"
  "rst|reStructuredText|doc" "adoc|AsciiDoc|doc" "asciidoc|AsciiDoc|doc"
  "tex|LaTeX|doc" "org|Org|doc" "license|Lizenz|doc" "licence|Lizenz|doc"
  "proto|Lizenz/Proto|doc" "log|Log|data"
  "mcs-notes|MCS-Notizen|doc" "tws-notes|TWS-Notizen|doc"
  "kernel-notes|Kernel-Notizen|doc" "dna-notes|DNA-Notizen|doc"
  "wiki-notes|Wiki-Notizen|doc" "vision-notes|Vision-Notizen|doc"
  "dev-note|Dev-Notizen|doc" "dev-notes|Dev-Notizen|doc"
  "install-notes|Install-Notizen|doc" "mcs-zertifikat|MCS-Zertifikat|doc"
  "pylovara-lizenz|Lizenz|doc" "lizenz|Lizenz|doc"
  "drittanbieter-lizenz|Lizenz|doc" "security-alert|Sicherheitsnotiz|doc"
  # Konfiguration / Daten
  "json|JSON|config" "jsonc|JSONC|config" "json5|JSON5|config"
  "yaml|YAML|config" "yml|YAML|config" "toml|TOML|config"
  "ini|INI|config" "conf|Konfiguration|config" "cfg|Konfiguration|config"
  "config|Konfiguration|config" "properties|Properties|config"
  "env|Env-Datei|config" "xml|XML|config" "lock|Lockfile|config"
  "gitattributes|Git-Attribute|config" "gitignore|Git-Ignore|config"
  "editorconfig|EditorConfig|config" "dockerignore|Docker-Ignore|config"
  "pv-conf|PV-Konfiguration|config" "last_check|Laufzeitstatus|data"
  "csv|CSV|data" "tsv|TSV|data" "ipynb|Notebook|data"
  # Binärmedien
  "pdf|PDF|binary" "jpg|JPEG|binary" "jpeg|JPEG|binary" "png|PNG|binary"
  "gif|GIF|binary" "webp|WebP|binary" "bmp|BMP|binary" "tiff|TIFF|binary"
  "ico|Icon|binary" "svg|SVG|binary" "psd|Photoshop|binary"
  "zip|ZIP|binary" "gz|GZip|binary" "bz2|BZip2|binary" "xz|XZ|binary"
  "7z|7-Zip|binary" "rar|RAR|binary" "tar|TAR|binary" "tgz|TGZ|binary"
  "mp3|MP3|binary" "mp4|MP4|binary" "wav|WAV|binary" "ogg|OGG|binary"
  "flac|FLAC|binary" "webm|WebM|binary" "mkv|MKV|binary" "avi|AVI|binary"
  "woff|Font|binary" "woff2|Font|binary" "ttf|Font|binary" "otf|Font|binary"
  "eot|Font|binary" "elf|ELF|binary" "exe|EXE|binary" "dll|DLL|binary"
  "so|Shared Library|binary" "o|Object|binary" "a|Archive|binary"
  "class|Java Class|binary" "jar|JAR|binary" "apk|APK|binary"
  "deb|Debian-Paket|binary" "rpm|RPM-Paket|binary" "dmg|DMG|binary"
  "iso|ISO|binary" "db|Datenbank|binary" "sqlite|SQLite|binary"
  "sqlite3|SQLite|binary" "pyc|Python-Bytecode|binary" "pyo|Python-Bytecode|binary"
  "wasm|WebAssembly|binary" "bin|Binärdatei|binary"
)

# Exakte Dateinamen ohne/ mit ungewöhnlicher Endung
BASE_RULES=(
  "mcs-shell|Bash|code"
  "systemd-service-unit|Systemd-Unit|config"
  "mcs-id|Braille-/ID-Daten|data"
  "tws-id|Braille-/ID-Daten|data"
  "connect-id|Hexagramm-/ID-Daten|data"
  "platzhalter|Platzhalter|data"
  "lexikalische-analyse|Notizen|doc"
  "git|Leer-/Hilfsdatei|unknown"
)

# Analyse-Ausschlüsse (Tooling, generierte Dateien, Laufzeitstatus)
EXCLUDE_DIRS=(.git .github node_modules target build dist vendor .cache __pycache__ .venv venv .idea .vscode)
EXCLUDE_FILES=(.mcs_analyzer.sh .mcs_status MCS-STATUS.md MCS-STATUS.svg .last_check)

ANALYZER_VERSION="2.0.0"
STATUS_FILE=".mcs_status"
REPORT_FILE="MCS-STATUS.md"
BADGE_FILE="MCS-STATUS.svg"
MARK_START="<!-- MCS-STATS:START -->"
MARK_END="<!-- MCS-STATS:END -->"

# -----------------------------------------------------------------------------
#  3) HILFSFUNKTIONEN
# -----------------------------------------------------------------------------
die() { printf 'FEHLER: %s\n' "$*" >&2; exit 2; }
fmt_int() {
  local n="${1:-0}" sign="" out="" len
  [[ "$n" == -* ]] && { sign="-"; n="${n#-}"; }
  len=${#n}
  while (( len > 3 )); do out=".${n:len-3:3}${out}"; n="${n:0:len-3}"; len=${#n}; done
  printf '%s%s%s' "$sign" "$n" "$out"
}
pct_x10() { # numerator denominator -> 0..1000
  local n="${1:-0}" d="${2:-0}"
  if (( d <= 0 )); then printf '0'; return; fi
  printf '%d' $(( (n * 1000 + d / 2) / d ))
}
pct_human() { local x="${1:-0}"; printf '%d,%d' $((x/10)) $((x%10)); }
plural_files() { local n="${1:-0}"; (( n == 1 )) && printf 'Datei' || printf 'Dateien'; }
bar() {
  local x="${1:-0}" width="${2:-24}" filled empty out=""
  filled=$(( (x * width + 500) / 1000 )); (( filled > width )) && filled=$width; (( filled < 0 )) && filled=0
  empty=$(( width - filled ))
  local i; for ((i=0;i<filled;i++)); do out+="█"; done
  for ((i=0;i<empty;i++)); do out+="░"; done
  printf '%s' "$out"
}
xml_escape() {
  local s="${1:-}"
  s=${s//&/&amp;}; s=${s//</&lt;}; s=${s//>/&gt;}; s=${s//\"/&quot;}
  printf '%s' "$s"
}
md_path() {
  local s="${1:-}"
  s=${s//|/\\|}; s=${s//\`/\\\`}
  printf '`%s`' "$s"
}

# -----------------------------------------------------------------------------
#  4) KONFIGURATION EINLESEN
# -----------------------------------------------------------------------------
declare -A EXT_NAME=() EXT_CAT=() BASE_NAME=() BASE_CAT=()
for rule in "${LANG_RULES[@]}"; do
  IFS='|' read -r k n c <<< "$rule"
  k="${k,,}"; EXT_NAME["$k"]="$n"; EXT_CAT["$k"]="$c"
done
for rule in "${BASE_RULES[@]}"; do
  IFS='|' read -r k n c <<< "$rule"
  k="${k,,}"; BASE_NAME["$k"]="$n"; BASE_CAT["$k"]="$c"
done

# -----------------------------------------------------------------------------
#  5) ARGUMENTE / REPO-ROOT
# -----------------------------------------------------------------------------
MODE="write"; ROOT_OVERRIDE=""; NO_README=0
while (( $# )); do
  case "$1" in
    --write) MODE="write" ;;
    --check) MODE="check" ;;
    --root) shift; [[ $# -gt 0 ]] || die "--root braucht ein Verzeichnis"; ROOT_OVERRIDE="$1" ;;
    --no-readme) NO_README=1 ;;
    -h|--help)
      cat <<'EOF'
Nutzung: .mcs_analyzer.sh [--write|--check] [--root DIR] [--no-readme]
  --write      Statusdateien schreiben (Standard)
  --check      Nur prüfen, ob Statusdateien aktuell sind (Exit 1 wenn nicht)
  --root DIR   Anderes Repository analysieren
  --no-readme  README-Dateien nicht anfassen
EOF
      exit 0 ;;
    *) die "Unbekannte Option: $1 (--help für Hilfe)" ;;
  esac
  shift
done

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
if [[ -n "$ROOT_OVERRIDE" ]]; then
  REPO_ROOT="$(cd -- "$ROOT_OVERRIDE" && pwd)"
elif git -C "$SCRIPT_DIR" rev-parse --show-toplevel >/dev/null 2>&1; then
  REPO_ROOT="$(git -C "$SCRIPT_DIR" rev-parse --show-toplevel)"
else
  REPO_ROOT="$SCRIPT_DIR"
fi
cd "$REPO_ROOT"

command -v awk >/dev/null || die "awk fehlt"
command -v sed >/dev/null || die "sed fehlt"
command -v sort >/dev/null || die "sort fehlt"
command -v file >/dev/null || printf 'WARNUNG: file fehlt, Binärerkennung nur über Endungen.\n' >&2

# -----------------------------------------------------------------------------
#  6) DATEIEN FINDEN
# -----------------------------------------------------------------------------
is_excluded() {
  local path="$1" f d part
  for f in "${EXCLUDE_FILES[@]}"; do [[ "$path" == "$f" ]] && return 0; done
  local IFS='/'; read -r -a parts <<< "$path"; unset IFS
  for part in "${parts[@]}"; do
    for d in "${EXCLUDE_DIRS[@]}"; do [[ "$part" == "$d" ]] && return 0; done
  done
  return 1
}
collect_files() {
  if git -C "$REPO_ROOT" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    git -C "$REPO_ROOT" ls-files -z --cached --others --exclude-standard
  else
    find . -type f -print0
  fi
}

# temporäres Arbeitsverzeichnis
TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/mcs-analyzer.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT
OUT_DIR="$REPO_ROOT"
if [[ "$MODE" == "check" ]]; then OUT_DIR="$TMP_DIR/out"; mkdir -p "$OUT_DIR"; fi

printf 'MCS-Analyzer %s | Root: %s | Modus: %s\n' "$ANALYZER_VERSION" "$REPO_ROOT" "$MODE"

# -----------------------------------------------------------------------------
#  7) MCS-PARSER (awk): findet nur vollständige Token-Paare, tolerant bei
#     Doku-Beispielen und Tippfehlern. Verschachtelung wird unterstützt.
# -----------------------------------------------------------------------------
AWK_FILE="$TMP_DIR/mcs_parse.awk"
cat > "$AWK_FILE" <<'AWK'
function trim(s) { sub(/^[ \t\r]+/, "", s); sub(/[ \t\r]+$/, "", s); return s }
function delimbase(s,   t) {
    t = trim(s); sub(/^\357\273\277/, "", t)
    if (allow_comment) { sub(/[ \t]+#.*$/, "", t); sub(/[ \t]+\/\/.*$/, "", t); t = trim(t) }
    return t
}
function is_open(s) { return (mode == "contains") ? (index(s, mcs_open) > 0) : (delimbase(s) == mcs_open) }
function is_close(s) { return (mode == "contains") ? (index(s, mcs_close) > 0) : (delimbase(s) == mcs_close) }
{
    total_lines = NR
    t = trim($0); nonempty[NR] = (t != ""); if (nonempty[NR]) nonempty_total++
    if (index($0, mcs_open) > 0 || index($0, mcs_close) > 0) token_lines++
    if (is_open($0)) {
        if (nesting || sp == 0) stack[++sp] = NR
    } else if (is_close($0)) {
        if (sp > 0) { o = stack[sp--]; matched_open[o] = NR; matched_close[NR] = o }
        else { orphan_closes++; orphan_close_lines = orphan_close_lines (orphan_close_lines == "" ? "" : ",") NR }
    }
}
END {
    orphan_opens = sp
    for (i = 1; i <= sp; i++) {
        unmatched[stack[i]] = 1
        orphan_open_lines = orphan_open_lines (orphan_open_lines == "" ? "" : ",") stack[i]
    }
    depth = 0; top_count = 0; frames_total = 0; mcs_content = 0; mcs_delims = 0
    mcs_span = 0; outside_nonempty = 0; cur_start = 0
    for (i = 1; i <= total_lines; i++) {
        if (i in matched_open) {
            if (depth == 0) { top_count++; cur_start = i }
            depth++; frames_total++; mcs_delims++
        } else if (i in matched_close) {
            mcs_delims++; depth--
            if (depth == 0) mcs_span += i - cur_start + 1
        } else if (count_unmatched && (i in unmatched)) {
            if (depth == 0) { top_count++; cur_start = i }
            depth++; frames_total++; mcs_delims++
        } else {
            if (depth > 0) { if (nonempty[i]) mcs_content++ }
            else { if (nonempty[i]) outside_nonempty++ }
        }
    }
    if (count_unmatched && depth > 0) mcs_span += total_lines - cur_start + 1
    printf "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n", \
        total_lines, nonempty_total, mcs_content, mcs_delims, mcs_span, \
        top_count, frames_total, orphan_opens, orphan_closes, outside_nonempty, \
        token_lines, (orphan_open_lines == "" ? "-" : orphan_open_lines), \
        (orphan_close_lines == "" ? "-" : orphan_close_lines)
}
AWK

# Optionaler Modus: Token irgendwo in der Zeile (z. B. kompakte Inline-Rahmen).
# Standard bleibt "line", weil Doku-Beispiele Tokens im Fließtext nennen.
AWK_CONTAINS_FILE="$TMP_DIR/mcs_parse_contains.awk"
cat > "$AWK_CONTAINS_FILE" <<'AWK'
function trim(s) { sub(/^[ \t\r]+/, "", s); sub(/[ \t\r]+$/, "", s); return s }
function mark_text(s, ln) {
    if (s ~ /^[ \t\r]*$/) return
    if (depth > 0) inside_line[ln] = 1
    else outside_line[ln] = 1
}
function scan(s, ln,   pos, len, rest, opos, cpos, typ, tpos, tlen, oline, odepth) {
    pos = 1; len = length(s)
    while (pos <= len) {
        rest = substr(s, pos)
        opos = index(rest, mcs_open)
        cpos = index(rest, mcs_close)
        if (opos == 0 && cpos == 0) break
        if (opos == 0) { typ = "C"; tpos = cpos; tlen = length(mcs_close) }
        else if (cpos == 0) { typ = "O"; tpos = opos; tlen = length(mcs_open) }
        else if (opos < cpos) { typ = "O"; tpos = opos; tlen = length(mcs_open) }
        else if (cpos < opos) { typ = "C"; tpos = cpos; tlen = length(mcs_close) }
        else { typ = "O"; tpos = opos; tlen = length(mcs_open) }
        mark_text(substr(rest, 1, tpos - 1), ln)
        token_lines[ln] = 1
        if (typ == "O") {
            if (nesting || sp == 0) {
                stack[++sp] = ln
                stack_depth[sp] = depth
                depth++
            }
        } else {
            if (sp > 0) {
                oline = stack[sp]; odepth = stack_depth[sp]
                sp--; depth--
                frames_total++; mcs_delims += 2
                if (odepth == 0) { top_count++; mcs_span += ln - oline + 1 }
            } else {
                orphan_closes++
                orphan_close_lines = orphan_close_lines (orphan_close_lines == "" ? "" : ",") ln
            }
        }
        pos = pos + tpos - 1 + tlen
    }
    mark_text(substr(s, pos), ln)
}
{
    total_lines = NR
    t = trim($0); nonempty[NR] = (t != ""); if (nonempty[NR]) nonempty_total++
    scan($0, NR)
}
END {
    orphan_opens = sp
    for (i = 1; i <= sp; i++) {
        unmatched[stack[i]] = 1
        orphan_open_lines = orphan_open_lines (orphan_open_lines == "" ? "" : ",") stack[i]
    }
    if (count_unmatched) {
        for (i = 1; i <= sp; i++) {
            frames_total++; mcs_delims++
            if (stack_depth[i] == 0) top_count++
            mcs_span += total_lines - stack[i] + 1
        }
    }
    for (i = 1; i <= total_lines; i++) {
        if (i in token_lines) token_lines_total++
        if (inside_line[i]) mcs_content++
        else if (outside_line[i]) outside_nonempty++
    }
    printf "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n", \
        total_lines, nonempty_total, mcs_content, mcs_delims, mcs_span, \
        top_count, frames_total, orphan_opens, orphan_closes, outside_nonempty, \
        token_lines_total, (orphan_open_lines == "" ? "-" : orphan_open_lines), \
        (orphan_close_lines == "" ? "-" : orphan_close_lines)
}
AWK

# -----------------------------------------------------------------------------
#  8) DATEIANALYSE
# -----------------------------------------------------------------------------
FILE_PATH=(); FILE_LANG=(); FILE_CAT=(); FILE_BYTES=(); FILE_TEXT=()
FILE_LINES=(); FILE_NONEMPTY=(); FILE_MCS_CONTENT=(); FILE_MCS_DELIMS=()
FILE_MCS_SPAN=(); FILE_FRAMES_TOP=(); FILE_FRAMES_TOTAL=(); FILE_ORPHAN_OPEN=()
FILE_ORPHAN_CLOSE=(); FILE_ORPHAN_OPEN_LINES=(); FILE_ORPHAN_CLOSE_LINES=()
FILE_TOKEN_LINES=(); FILE_OUTSIDE=(); FILE_COVERAGE=(); FILE_KIND=()

file_is_binary() {
  local path="$1" ext="$2" mime="$3" desc="$4"
  [[ -n "$ext" && "${EXT_CAT[$ext]:-}" == "binary" ]] && return 0
  case "$mime" in
    text/*|application/json|application/xml|application/x-sh|application/x-shellscript|application/x-python|application/javascript|application/x-php|application/x-ruby|application/x-perl|application/x-lua|application/toml|application/yaml|application/x-yaml|inode/x-empty|application/x-empty) return 1 ;;
    application/pdf|image/*|audio/*|video/*|font/*|application/zip|application/gzip|application/x-xz|application/x-7z-compressed|application/x-rar-compressed|application/x-tar|application/octet-stream|application/x-executable|application/x-pie-executable|application/x-sharedlib|application/x-object|application/x-archive|application/java-archive) return 0 ;;
  esac
  case "$desc" in
    *ELF*|*"PDF document"*|*image*|*archive*|*font*|*executable*|*"shared object"*) return 0 ;;
    *text*|*script*|*source*|*JSON*|*XML*|*empty*) return 1 ;;
  esac
  if command -v grep >/dev/null && LC_ALL=C grep -q $'\x00' -- "$path" 2>/dev/null; then return 0; fi
  return 1
}

detect_language() {
  local path="$1" base lower ext first desc="$2"
  base="$(basename -- "$path")"; lower="${base,,}"; ext=""
  [[ "$lower" == *.* ]] && ext="${lower##*.}"
  if [[ -n "${BASE_CAT[$lower]:-}" ]]; then
    LANG_NAME="${BASE_NAME[$lower]}"; LANG_CAT="${BASE_CAT[$lower]}"; return
  fi
  if [[ -n "$ext" && -n "${EXT_CAT[$ext]:-}" ]]; then
    LANG_NAME="${EXT_NAME[$ext]}"; LANG_CAT="${EXT_CAT[$ext]}"; return
  fi
  case "$lower" in
    *-notes|*-note) LANG_NAME="Notizen"; LANG_CAT="doc"; return ;;
    *-lizenz) LANG_NAME="Lizenz"; LANG_CAT="doc"; return ;;
    *-zertifikat) LANG_NAME="MCS-Zertifikat"; LANG_CAT="doc"; return ;;
    *-alert) LANG_NAME="Sicherheitsnotiz"; LANG_CAT="doc"; return ;;
    *-id) LANG_NAME="ID-/Braille-Daten"; LANG_CAT="data"; return ;;
    systemd-*) LANG_NAME="Systemd-Unit"; LANG_CAT="config"; return ;;
  esac
  first="$(head -n 1 -- "$path" 2>/dev/null || true)"
  case "$first" in
    '#!'*bash*|'#!'*'/sh'*|'#!'*zsh*|'#!'*dash*|'#!'*ksh*) LANG_NAME="Bash/Shell"; LANG_CAT="code"; return ;;
    '#!'*python*) LANG_NAME="Python"; LANG_CAT="code"; return ;;
    '#!'*php*) LANG_NAME="PHP"; LANG_CAT="code"; return ;;
    '#!'*node*|'#!'*deno*|'#!'*bun*) LANG_NAME="JavaScript/TypeScript"; LANG_CAT="code"; return ;;
    '#!'*ruby*) LANG_NAME="Ruby"; LANG_CAT="code"; return ;;
    '#!'*perl*) LANG_NAME="Perl"; LANG_CAT="code"; return ;;
    '#!'*lua*) LANG_NAME="Lua"; LANG_CAT="code"; return ;;
    '#!'*Rscript*|'#!'*rscript*) LANG_NAME="R"; LANG_CAT="code"; return ;;
    '#!'*pwsh*|'#!'*powershell*) LANG_NAME="PowerShell"; LANG_CAT="code"; return ;;
    '#!'*awk*) LANG_NAME="AWK"; LANG_CAT="code"; return ;;
    '#!'*sed*) LANG_NAME="Sed"; LANG_CAT="code"; return ;;
  esac
  case "$desc" in
    *"Bourne-Again shell script"*|*"POSIX shell script"*|*"shell script"*) LANG_NAME="Bash/Shell"; LANG_CAT="code"; return ;;
    *"Python script"*) LANG_NAME="Python"; LANG_CAT="code"; return ;;
    *"PHP script"*) LANG_NAME="PHP"; LANG_CAT="code"; return ;;
    *"Perl script"*) LANG_NAME="Perl"; LANG_CAT="code"; return ;;
    *"Ruby script"*) LANG_NAME="Ruby"; LANG_CAT="code"; return ;;
    *"C source"*) LANG_NAME="C"; LANG_CAT="code"; return ;;
    *"C++ source"*) LANG_NAME="C++"; LANG_CAT="code"; return ;;
  esac
  if [[ ! -s "$path" && -z "$first" ]]; then LANG_NAME="Leer/Unbekannt"; LANG_CAT="unknown"; return; fi
  LANG_NAME="Text/Daten"; LANG_CAT="data"
}

analyze_file() {
  local path="$1" idx="$2" mime="" desc="" binary=0 ext base lower
  base="$(basename -- "$path")"; lower="${base,,}"; ext=""
  [[ "$lower" == *.* ]] && ext="${lower##*.}"
  if command -v file >/dev/null; then
    mime="$(LC_ALL=C file -b --mime-type -- "$path" 2>/dev/null || true)"
    desc="$(LC_ALL=C file -b -- "$path" 2>/dev/null || true)"
  fi
  file_is_binary "$path" "$ext" "$mime" "$desc" && binary=1

  FILE_PATH[$idx]="$path"
  FILE_BYTES[$idx]="$(wc -c < "$path" 2>/dev/null | tr -d '[:space:]' || echo 0)"
  if (( binary )); then
    FILE_TEXT[$idx]=0; FILE_LANG[$idx]="Binärdatei"; FILE_CAT[$idx]="binary"
    case "$desc" in
      *ELF*) FILE_LANG[$idx]="ELF-Binärdatei" ;;
      *"PDF document"*) FILE_LANG[$idx]="PDF" ;;
      *image*|*JPEG*|*PNG*) FILE_LANG[$idx]="Bild" ;;
      *archive*|*Zip*|*gzip*) FILE_LANG[$idx]="Archiv" ;;
      *font*) FILE_LANG[$idx]="Schrift" ;;
    esac
    FILE_LINES[$idx]=0; FILE_NONEMPTY[$idx]=0; FILE_MCS_CONTENT[$idx]=0
    FILE_MCS_DELIMS[$idx]=0; FILE_MCS_SPAN[$idx]=0; FILE_FRAMES_TOP[$idx]=0
    FILE_FRAMES_TOTAL[$idx]=0; FILE_ORPHAN_OPEN[$idx]=0; FILE_ORPHAN_CLOSE[$idx]=0
    FILE_ORPHAN_OPEN_LINES[$idx]="-"; FILE_ORPHAN_CLOSE_LINES[$idx]="-"
    FILE_TOKEN_LINES[$idx]=0; FILE_OUTSIDE[$idx]=0; FILE_COVERAGE[$idx]=0
    FILE_KIND[$idx]="binary"
    return
  fi

  FILE_TEXT[$idx]=1
  LANG_NAME=""; LANG_CAT=""
  detect_language "$path" "$desc"
  FILE_LANG[$idx]="$LANG_NAME"; FILE_CAT[$idx]="$LANG_CAT"

  local out total nonempty content delims span ftop ftotal oopen oclose outside tokens oopenl oclosel
  local parse_path="$path" filtered="" awk_parser="$AWK_FILE"
  [[ "$MCS_DELIM_MODE" == "contains" ]] && awk_parser="$AWK_CONTAINS_FILE"
  if (( MCS_IGNORE_MARKER_BLOCKS )) && grep -qF "$MARK_START" "$path" 2>/dev/null; then
    filtered="$TMP_DIR/filtered.$idx"
    awk -v s="$MARK_START" -v e="$MARK_END" '$0==s{skip=1;next} $0==e{skip=0;next} !skip' "$path" > "$filtered"
    parse_path="$filtered"
  fi
  out="$(awk -v mcs_open="$MCS_OPEN" -v mcs_close="$MCS_CLOSE" \
            -v mode="$MCS_DELIM_MODE" -v allow_comment="$MCS_ALLOW_TRAILING_COMMENT" \
            -v nesting="$MCS_NESTING" -v count_unmatched="$MCS_COUNT_UNMATCHED" \
            -f "$awk_parser" "$parse_path" 2>/dev/null || true)"
  if [[ -z "$out" ]]; then
    out=$'0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t-\t-'
  fi
  IFS=$'\t' read -r total nonempty content delims span ftop ftotal oopen oclose outside tokens oopenl oclosel <<< "$out"
  FILE_LINES[$idx]="${total:-0}"; FILE_NONEMPTY[$idx]="${nonempty:-0}"
  FILE_MCS_CONTENT[$idx]="${content:-0}"; FILE_MCS_DELIMS[$idx]="${delims:-0}"
  FILE_MCS_SPAN[$idx]="${span:-0}"; FILE_FRAMES_TOP[$idx]="${ftop:-0}"
  FILE_FRAMES_TOTAL[$idx]="${ftotal:-0}"; FILE_ORPHAN_OPEN[$idx]="${oopen:-0}"
  FILE_ORPHAN_CLOSE[$idx]="${oclose:-0}"; FILE_OUTSIDE[$idx]="${outside:-0}"
  FILE_TOKEN_LINES[$idx]="${tokens:-0}"; FILE_ORPHAN_OPEN_LINES[$idx]="${oopenl:--}"
  FILE_ORPHAN_CLOSE_LINES[$idx]="${oclosel:--}"
  FILE_COVERAGE[$idx]=$(pct_x10 $(( ${FILE_MCS_CONTENT[$idx]} + ${FILE_MCS_DELIMS[$idx]} )) "${FILE_NONEMPTY[$idx]}")
  (( ${FILE_COVERAGE[$idx]} > 1000 )) && FILE_COVERAGE[$idx]=1000
  if (( ${FILE_FRAMES_TOP[$idx]} > 0 )); then
    if (( ${FILE_COVERAGE[$idx]} >= MCS_DOMINANT_THRESHOLD * 10 )); then
      FILE_KIND[$idx]="dominant"
    else
      FILE_KIND[$idx]="mixed"
    fi
  elif (( ${FILE_TOKEN_LINES[$idx]} > 0 )); then
    FILE_KIND[$idx]="trace"
  else
    FILE_KIND[$idx]="none"
  fi
}

# -----------------------------------------------------------------------------
#  9) ALLE DATEIEN EINLESEN
# -----------------------------------------------------------------------------
FILE_COUNT=0
while IFS= read -r -d '' rel; do
  [[ -z "$rel" ]] && continue
  is_excluded "$rel" && continue
  [[ -f "$rel" ]] || continue
  FILE_COUNT=$((FILE_COUNT + 1))
  analyze_file "$rel" "$FILE_COUNT"
done < <(collect_files)

printf 'Analysierte Dateien: %d\n' "$FILE_COUNT"

# -----------------------------------------------------------------------------
# 10) AGGREGATION
# -----------------------------------------------------------------------------
declare -A LANG_FILES=() LANG_LINES=() LANG_CAT=()
declare -A CAT_FILES=() CAT_LINES=() CAT_BYTES=()

TOTAL_FILES=0 TEXT_FILES=0 BINARY_FILES=0 TOTAL_LINES=0 TOTAL_NONEMPTY=0
MCS_FILES=0 MCS_DOMINANT=0 MCS_MIXED=0 MCS_TRACE=0
MCS_FRAMES_TOP=0 MCS_FRAMES_TOTAL=0 MCS_CONTENT=0 MCS_DELIMS=0 MCS_LINES=0 MCS_SPAN=0
MCS_LINES_DOMINANT=0 MCS_LINES_MIXED=0
FOREIGN_FILES=0 FOREIGN_LINES=0 DOC_FILES=0 DOC_LINES=0 CONFIG_FILES=0 CONFIG_LINES=0
DATA_FILES=0 DATA_LINES=0 UNKNOWN_FILES=0 UNKNOWN_LINES=0 BINARY_BYTES=0

add_lang() { local l="$1" n="$2" c="$3"; LANG_FILES["$l"]=$(( ${LANG_FILES["$l"]:-0} + 1 )); LANG_LINES["$l"]=$(( ${LANG_LINES["$l"]:-0} + n )); LANG_CAT["$l"]="$c"; }
add_cat_line() { local c="$1" n="$2"; CAT_FILES["$c"]=$(( ${CAT_FILES["$c"]:-0} + 1 )); CAT_LINES["$c"]=$(( ${CAT_LINES["$c"]:-0} + n )); }

i=0
while (( i < FILE_COUNT )); do
  i=$((i + 1))
  TOTAL_FILES=$((TOTAL_FILES + 1))
  lang="${FILE_LANG[$i]}"; cat="${FILE_CAT[$i]}"; kind="${FILE_KIND[$i]}"
  if [[ "$kind" == "binary" ]]; then
    BINARY_FILES=$((BINARY_FILES + 1))
    BINARY_BYTES=$((BINARY_BYTES + ${FILE_BYTES[$i]}))
    CAT_FILES["binary"]=$(( ${CAT_FILES["binary"]:-0} + 1 ))
    CAT_BYTES["binary"]=$(( ${CAT_BYTES["binary"]:-0} + ${FILE_BYTES[$i]} ))
    LANG_FILES["$lang"]=$(( ${LANG_FILES["$lang"]:-0} + 1 ))
    LANG_CAT["$lang"]="binary"
    continue
  fi

  TEXT_FILES=$((TEXT_FILES + 1))
  TOTAL_LINES=$((TOTAL_LINES + ${FILE_LINES[$i]}))
  TOTAL_NONEMPTY=$((TOTAL_NONEMPTY + ${FILE_NONEMPTY[$i]}))
  LANG_FILES["$lang"]=$(( ${LANG_FILES["$lang"]:-0} + 1 ))
  LANG_CAT["$lang"]="$cat"

  outside="${FILE_OUTSIDE[$i]}"
  if [[ "$kind" == "dominant" || "$kind" == "mixed" ]]; then
    MCS_FILES=$((MCS_FILES + 1))
    if [[ "$kind" == "dominant" ]]; then
      MCS_DOMINANT=$((MCS_DOMINANT + 1))
      MCS_LINES_DOMINANT=$((MCS_LINES_DOMINANT + ${FILE_MCS_CONTENT[$i]} + ${FILE_MCS_DELIMS[$i]}))
    else
      MCS_MIXED=$((MCS_MIXED + 1))
      MCS_LINES_MIXED=$((MCS_LINES_MIXED + ${FILE_MCS_CONTENT[$i]} + ${FILE_MCS_DELIMS[$i]}))
    fi
    MCS_FRAMES_TOP=$((MCS_FRAMES_TOP + ${FILE_FRAMES_TOP[$i]}))
    MCS_FRAMES_TOTAL=$((MCS_FRAMES_TOTAL + ${FILE_FRAMES_TOTAL[$i]}))
    MCS_CONTENT=$((MCS_CONTENT + ${FILE_MCS_CONTENT[$i]}))
    MCS_DELIMS=$((MCS_DELIMS + ${FILE_MCS_DELIMS[$i]}))
    MCS_SPAN=$((MCS_SPAN + ${FILE_MCS_SPAN[$i]}))
    MCS_LINES=$((MCS_LINES + ${FILE_MCS_CONTENT[$i]} + ${FILE_MCS_DELIMS[$i]}))
    LANG_LINES["$lang"]=$(( ${LANG_LINES["$lang"]:-0} + outside ))
    if (( outside > 0 )); then
      case "$cat" in
        code) FOREIGN_FILES=$((FOREIGN_FILES + 1)); FOREIGN_LINES=$((FOREIGN_LINES + outside)) ;;
        doc) DOC_FILES=$((DOC_FILES + 1)); DOC_LINES=$((DOC_LINES + outside)) ;;
        config) CONFIG_FILES=$((CONFIG_FILES + 1)); CONFIG_LINES=$((CONFIG_LINES + outside)) ;;
        data) DATA_FILES=$((DATA_FILES + 1)); DATA_LINES=$((DATA_LINES + outside)) ;;
        unknown) UNKNOWN_FILES=$((UNKNOWN_FILES + 1)); UNKNOWN_LINES=$((UNKNOWN_LINES + outside)) ;;
        *) DOC_FILES=$((DOC_FILES + 1)); DOC_LINES=$((DOC_LINES + outside)) ;;
      esac
    fi
    continue
  fi

  [[ "$kind" == "trace" ]] && MCS_TRACE=$((MCS_TRACE + 1))

  LANG_LINES["$lang"]=$(( ${LANG_LINES["$lang"]:-0} + ${FILE_NONEMPTY[$i]} ))
  case "$cat" in
    code) FOREIGN_FILES=$((FOREIGN_FILES + 1)); FOREIGN_LINES=$((FOREIGN_LINES + ${FILE_NONEMPTY[$i]})) ;;
    doc) DOC_FILES=$((DOC_FILES + 1)); DOC_LINES=$((DOC_LINES + ${FILE_NONEMPTY[$i]})) ;;
    config) CONFIG_FILES=$((CONFIG_FILES + 1)); CONFIG_LINES=$((CONFIG_LINES + ${FILE_NONEMPTY[$i]})) ;;
    data) DATA_FILES=$((DATA_FILES + 1)); DATA_LINES=$((DATA_LINES + ${FILE_NONEMPTY[$i]})) ;;
    unknown) UNKNOWN_FILES=$((UNKNOWN_FILES + 1)); UNKNOWN_LINES=$((UNKNOWN_LINES + ${FILE_NONEMPTY[$i]})) ;;
    *) DOC_FILES=$((DOC_FILES + 1)); DOC_LINES=$((DOC_LINES + ${FILE_NONEMPTY[$i]})) ;;
  esac
done

LANG_FILES["MCS"]="$MCS_FILES"; LANG_LINES["MCS"]="$MCS_LINES"; LANG_CAT["MCS"]="mcs"
CAT_FILES["mcs"]="$MCS_FILES"; CAT_LINES["mcs"]="$MCS_LINES"

CODE_TOTAL=$((MCS_LINES + FOREIGN_LINES))
MCS_CODE_X10=$(pct_x10 "$MCS_LINES" "$CODE_TOTAL")
FOREIGN_CODE_X10=$(( CODE_TOTAL > 0 ? 1000 - MCS_CODE_X10 : 0 ))
ACTIVE_TEXT=$((MCS_LINES + FOREIGN_LINES + DOC_LINES + CONFIG_LINES + UNKNOWN_LINES))
(( ACTIVE_TEXT > 0 )) || ACTIVE_TEXT=1
MCS_TEXT_X10=$(pct_x10 "$MCS_LINES" "$ACTIVE_TEXT")
DOC_TEXT_X10=$(pct_x10 "$DOC_LINES" "$ACTIVE_TEXT")
CONFIG_TEXT_X10=$(pct_x10 "$CONFIG_LINES" "$ACTIVE_TEXT")
DATA_TEXT_X10=$(pct_x10 "$DATA_LINES" "$TOTAL_NONEMPTY")
MCS_FRAMES_NESTED=$((MCS_FRAMES_TOTAL - MCS_FRAMES_TOP))
[[ "$MCS_FRAMES_NESTED" -lt 0 ]] && MCS_FRAMES_NESTED=0
MCS_STATUS="NICHT ERKANNT"; [[ "$MCS_FILES" -gt 0 ]] && MCS_STATUS="ERKANNT"

# Quell-Commit (stabil; Statusdateien zählen nicht als Quelländerung)
SOURCE_COMMIT=""; SOURCE_EPOCH=""
if git -C "$REPO_ROOT" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  SOURCE_COMMIT="$(git -C "$REPO_ROOT" log -1 --format='%h' -- . \
    ':(exclude).mcs_status' ':(exclude)MCS-STATUS.md' ':(exclude)MCS-STATUS.svg' \
    ':(exclude)README.md' ':(exclude)README.WIKI-NOTES' 2>/dev/null || true)"
  SOURCE_EPOCH="$(git -C "$REPO_ROOT" log -1 --format='%ct' -- . \
    ':(exclude).mcs_status' ':(exclude)MCS-STATUS.md' ':(exclude)MCS-STATUS.svg' \
    ':(exclude)README.md' ':(exclude)README.WIKI-NOTES' 2>/dev/null || true)"
fi
[[ -n "$SOURCE_COMMIT" ]] || SOURCE_COMMIT="ohne-commit"
[[ -n "$SOURCE_EPOCH" ]] || SOURCE_EPOCH="$(date -u +%s)"
SOURCE_DATE="$(date -u -d "@$SOURCE_EPOCH" '+%Y-%m-%d %H:%M:%S UTC' 2>/dev/null || date -u -r "$SOURCE_EPOCH" '+%Y-%m-%d %H:%M:%S UTC' 2>/dev/null || echo unbekannt)"

# -----------------------------------------------------------------------------
# 11) AUSGABE-BAUSTEINE
# -----------------------------------------------------------------------------
NONCODE_TEXT=$((DOC_LINES + CONFIG_LINES + UNKNOWN_LINES))
(( NONCODE_TEXT > 0 )) || NONCODE_TEXT=1

lang_rows() {
  local filter="$1" denom="$2" tsv="$TMP_DIR/langs.$$.$RANDOM" n f l c
  : > "$tsv"
  for l in "${!LANG_LINES[@]}"; do
    n="${LANG_LINES[$l]:-0}"; (( n > 0 )) || continue
    c="${LANG_CAT[$l]:-unknown}"
    case "$filter" in
      code) [[ "$c" == "code" || "$c" == "mcs" ]] || continue ;;
      text) [[ "$c" == "doc" || "$c" == "config" || "$c" == "unknown" ]] || continue ;;
      data) [[ "$c" == "data" ]] || continue ;;
    esac
    printf '%s\t%s\t%s\t%s\n' "$n" "${LANG_FILES[$l]:-0}" "$l" "$c" >> "$tsv"
  done
  sort -t$'\t' -k1,1nr -k3,3 "$tsv" | while IFS=$'\t' read -r n f l c; do
    printf '| %s | %s | %s | %s | %s %% |\n' "$l" "$c" "$(fmt_int "$f")" "$(fmt_int "$n")" "$(pct_human "$(pct_x10 "$n" "$denom")")"
  done
}

mcs_rows() {
  local tsv="$TMP_DIR/mcsfiles.$$.$RANDOM" i n kind cov
  : > "$tsv"
  for ((i=1; i<=FILE_COUNT; i++)); do
    kind="${FILE_KIND[$i]}"
    [[ "$kind" == "dominant" || "$kind" == "mixed" ]] || continue
    n=$(( ${FILE_MCS_CONTENT[$i]} + ${FILE_MCS_DELIMS[$i]} ))
    printf '%s\t%s\t%s\t%s\t%s\n' "$n" "${FILE_FRAMES_TOP[$i]}" "${FILE_COVERAGE[$i]}" "${FILE_PATH[$i]}" "$kind" >> "$tsv"
  done
  sort -t$'\t' -k1,1nr -k4,4 "$tsv" | while IFS=$'\t' read -r n frames cov path kind; do
    printf '| %s | %s | %s | %s %% | %s |\n' "$(md_path "$path")" "$(fmt_int "$frames")" "$(fmt_int "$n")" "$(pct_human "$cov")" "$([[ "$kind" == dominant ]] && echo dominant || echo gemischt)"
  done
}

warning_rows() {
  local i shown=0 parts joined p
  for ((i=1; i<=FILE_COUNT; i++)); do
    if (( ${FILE_ORPHAN_OPEN[$i]:-0} > 0 || ${FILE_ORPHAN_CLOSE[$i]:-0} > 0 )); then
      parts=(); joined=""
      if (( ${FILE_ORPHAN_OPEN[$i]:-0} > 0 )); then
        p="${FILE_ORPHAN_OPEN[$i]} unpaarige \`$MCS_OPEN\`"
        [[ "${FILE_ORPHAN_OPEN_LINES[$i]:--}" != "-" ]] && p+=" (Zeile ${FILE_ORPHAN_OPEN_LINES[$i]})"
        parts+=("$p")
      fi
      if (( ${FILE_ORPHAN_CLOSE[$i]:-0} > 0 )); then
        p="${FILE_ORPHAN_CLOSE[$i]} unpaarige \`$MCS_CLOSE\`"
        [[ "${FILE_ORPHAN_CLOSE_LINES[$i]:--}" != "-" ]] && p+=" (Zeile ${FILE_ORPHAN_CLOSE_LINES[$i]})"
        parts+=("$p")
      fi
      for p in "${parts[@]}"; do joined+="${joined:+, }$p"; done
      printf -- '- %s: %s\n' "$(md_path "${FILE_PATH[$i]}")" "$joined"
      shown=$((shown + 1))
    fi
  done
  for ((i=1; i<=FILE_COUNT; i++)); do
    [[ "${FILE_KIND[$i]}" == "trace" ]] || continue
    (( ${FILE_ORPHAN_OPEN[$i]:-0} > 0 || ${FILE_ORPHAN_CLOSE[$i]:-0} > 0 )) && continue
    printf -- '- %s: `%s`/`%s` nur als Textspur/Beispiel, kein vollständiger Transaktionsrahmen.\n' \
      "$(md_path "${FILE_PATH[$i]}")" "$MCS_OPEN" "$MCS_CLOSE"
    shown=$((shown + 1))
  done
  if (( shown == 0 )); then printf -- '- Keine Syntaxauffälligkeiten.\n'; fi
}

missing_code_langs() {
  local rule k n c out="" seen=""
  declare -A seen_a=()
  for rule in "${LANG_RULES[@]}"; do
    IFS='|' read -r k n c <<< "$rule"
    [[ "$c" == "code" ]] || continue
    [[ -n "${seen_a[$n]:-}" ]] && continue
    seen_a["$n"]=1
    (( ${LANG_LINES[$n]:-0} == 0 )) && out+="${out:+, }$n"
  done
  printf '%s' "${out:-keine}"
}

build_status() {
cat <<EOF
[TWS-STATUS: AKTIV]
MCS-STATUS: $MCS_STATUS
MCS-MODULE: $MCS_FILES
MCS-FILES: $MCS_FILES
MCS-DOMINANT-FILES: $MCS_DOMINANT
MCS-MIXED-FILES: $MCS_MIXED
MCS-TRACE-FILES: $MCS_TRACE
MCS-FRAMES: $MCS_FRAMES_TOTAL
MCS-FRAMES-TOP: $MCS_FRAMES_TOP
MCS-FRAMES-NESTED: $MCS_FRAMES_NESTED
MCS-LINES: $MCS_LINES
MCS-CONTENT-LINES: $MCS_CONTENT
MCS-DELIM-LINES: $MCS_DELIMS
MCS-DOMINANT-LINES: $MCS_LINES_DOMINANT
MCS-MIXED-LINES: $MCS_LINES_MIXED
MCS-SPAN-LINES: $MCS_SPAN
MCS-PERCENT-CODE: $(pct_human "$MCS_CODE_X10")%
MCS-PERCENT-TEXT: $(pct_human "$MCS_TEXT_X10")%
ACTIVE-TEXT-LINES: $ACTIVE_TEXT
FOREIGN-FILES: $FOREIGN_FILES
FOREIGN-LINES: $FOREIGN_LINES
FOREIGN-PERCENT-CODE: $(pct_human "$FOREIGN_CODE_X10")%
DOC-FILES: $DOC_FILES
DOC-LINES: $DOC_LINES
CONFIG-FILES: $CONFIG_FILES
CONFIG-LINES: $CONFIG_LINES
DATA-FILES: $DATA_FILES
DATA-LINES: $DATA_LINES
BINARY-FILES: $BINARY_FILES
BINARY-BYTES: $BINARY_BYTES
TOTAL-FILES: $TOTAL_FILES
TEXT-FILES: $TEXT_FILES
TOTAL-LINES: $TOTAL_LINES
TOTAL-NONEMPTY-LINES: $TOTAL_NONEMPTY
MCS-ANTEIL: $(pct_human "$MCS_CODE_X10")%
DOC-ANTEIL: $(pct_human "$(pct_x10 "$DOC_LINES" "$NONCODE_TEXT")")%
OTHER-ANTEIL: $(pct_human "$(pct_x10 "$((CONFIG_LINES + UNKNOWN_LINES))" "$NONCODE_TEXT")")%
SOURCE-COMMIT: $SOURCE_COMMIT
TIMESTAMP: $SOURCE_DATE
ANALYZER: $ANALYZER_VERSION
MCS-SYNTAX-OPEN: $MCS_OPEN
MCS-SYNTAX-CLOSE: $MCS_CLOSE
MCS-DELIM-MODE: $MCS_DELIM_MODE
MCS-NESTING: $MCS_NESTING
EOF
}

build_wiki() {
cat <<EOF
 PNS PYLOVARA-SYSTEM // TWS ARCHITEKTUR-METRIK
--------------------------------------------------
 MCS-STATUS        : $MCS_STATUS
 MCS-DATEIEN       : $(fmt_int "$MCS_FILES") ($(fmt_int "$MCS_DOMINANT") dominant, $(fmt_int "$MCS_MIXED") gemischt)
 MCS-RAHMEN        : $(fmt_int "$MCS_FRAMES_TOTAL") ($(fmt_int "$MCS_FRAMES_TOP") top-level, $(fmt_int "$MCS_FRAMES_NESTED") verschachtelt)
 MCS-ZEILEN        : $(fmt_int "$MCS_LINES") ($(fmt_int "$MCS_CONTENT") Inhalt + $(fmt_int "$MCS_DELIMS") Rahmen)
 MCS-ANTEIL CODE   : $(pct_human "$MCS_CODE_X10") %
 MCS-ANTEIL TEXT   : $(pct_human "$MCS_TEXT_X10") % (aktiver Text)
 FREMDCODE         : $(fmt_int "$FOREIGN_LINES") Zeilen ($(fmt_int "$FOREIGN_FILES") Dateien)
 DOKU/KONFIG       : $(fmt_int "$NONCODE_TEXT") Zeilen
 MASSENDATEN       : $(fmt_int "$DATA_LINES") Zeilen (nicht in Prozenten)
 BINÄRMEDIEN       : $(fmt_int "$BINARY_FILES") Dateien
 MCS-SPUREN        : $(fmt_int "$MCS_TRACE") $(plural_files "$MCS_TRACE") (nur Beispiele/Text)
 QUELL-COMMIT      : $SOURCE_COMMIT
 STAND             : $SOURCE_DATE
EOF
}

build_readme() {
cat <<EOF
## MCS-Substanz (automatisch)

**MCS $MCS_STATUS** · $(fmt_int "$MCS_FILES") $(plural_files "$MCS_FILES") · $(fmt_int "$MCS_FRAMES_TOTAL") Rahmen · $(fmt_int "$MCS_LINES") MCS-Zeilen

\`\`\`text
MCS   $(bar "$MCS_CODE_X10" 24)  $(pct_human "$MCS_CODE_X10") % vom Code
Fremd $(bar "$FOREIGN_CODE_X10" 24)  $(pct_human "$FOREIGN_CODE_X10") % vom Code
\`\`\`

| Bereich | Dateien | Zeilen | Anteil |
|---|---:|---:|---:|
| MCS (Eigen) | $(fmt_int "$MCS_FILES") | $(fmt_int "$MCS_LINES") | $(pct_human "$MCS_CODE_X10") % vom Code |
| Fremdcode | $(fmt_int "$FOREIGN_FILES") | $(fmt_int "$FOREIGN_LINES") | $(pct_human "$FOREIGN_CODE_X10") % vom Code |
| Dokumentation | $(fmt_int "$DOC_FILES") | $(fmt_int "$DOC_LINES") | $(pct_human "$(pct_x10 "$DOC_LINES" "$NONCODE_TEXT")") % vom Doku-/Konfig-Text |
| Konfiguration | $(fmt_int "$CONFIG_FILES") | $(fmt_int "$CONFIG_LINES") | $(pct_human "$(pct_x10 "$CONFIG_LINES" "$NONCODE_TEXT")") % vom Doku-/Konfig-Text |
| Massendaten | $(fmt_int "$DATA_FILES") | $(fmt_int "$DATA_LINES") | – (nicht in Prozenten) |

*$(fmt_int "$MCS_LINES_DOMINANT") der $(fmt_int "$MCS_LINES") MCS-Zeilen stehen in MCS-dominanten Dateien, $(fmt_int "$MCS_LINES_MIXED") in gemischten Doku-/Notizdateien.*
*MCS-Anteil am aktiven Text: $(pct_human "$MCS_TEXT_X10") % (ohne Massendaten).*

[Vollständiger MCS-Status](MCS-STATUS.md) · Stand: $SOURCE_DATE
EOF
}

build_report() {
cat <<EOF
# MCS-STATUS

> Automatisch erzeugt durch \`.mcs_analyzer.sh\` ($ANALYZER_VERSION).
> Syntax: \`$MCS_OPEN\` … \`$MCS_CLOSE\` · Modus: \`$MCS_DELIM_MODE\` · Verschachtelung: $MCS_NESTING
> Quell-Commit: \`$SOURCE_COMMIT\` · Stand: $SOURCE_DATE

## Kurzstatus

**MCS $MCS_STATUS** — $(fmt_int "$MCS_FILES") $(plural_files "$MCS_FILES") mit gültigen Transaktionsrahmen.

\`\`\`text
MCS   $(bar "$MCS_CODE_X10" 24)  $(pct_human "$MCS_CODE_X10") % vom Code
Fremd $(bar "$FOREIGN_CODE_X10" 24)  $(pct_human "$FOREIGN_CODE_X10") % vom Code
\`\`\`

| Kennzahl | Wert |
|---|---:|
| MCS-Dateien | $(fmt_int "$MCS_FILES") |
| davon MCS-dominant (≥ $MCS_DOMINANT_THRESHOLD %) | $(fmt_int "$MCS_DOMINANT") |
| davon MCS-gemischt | $(fmt_int "$MCS_MIXED") |
| MCS-Transaktionsrahmen | $(fmt_int "$MCS_FRAMES_TOTAL") ($(fmt_int "$MCS_FRAMES_TOP") top-level, $(fmt_int "$MCS_FRAMES_NESTED") verschachtelt) |
| MCS-Zeilen gesamt | $(fmt_int "$MCS_LINES") |
| – davon Inhalt | $(fmt_int "$MCS_CONTENT") |
| – davon Rahmensymbole | $(fmt_int "$MCS_DELIMS") |
| – davon in MCS-dominanten Dateien | $(fmt_int "$MCS_LINES_DOMINANT") |
| – davon in gemischten Dateien | $(fmt_int "$MCS_LINES_MIXED") |
| MCS-Anteil am aktiven Text | $(pct_human "$MCS_TEXT_X10") % |
| MCS-Anteil am Code | $(pct_human "$MCS_CODE_X10") % |
| Fremdcode | $(fmt_int "$FOREIGN_LINES") Zeilen in $(fmt_int "$FOREIGN_FILES") $(plural_files "$FOREIGN_FILES") |
| Dokumentation | $(fmt_int "$DOC_LINES") Zeilen |
| Konfiguration | $(fmt_int "$CONFIG_LINES") Zeilen |
| Massendaten (nicht in Prozenten) | $(fmt_int "$DATA_LINES") Zeilen |
| MCS-Spuren (nur Beispiele/Text) | $(fmt_int "$MCS_TRACE") $(plural_files "$MCS_TRACE") |
| Textdateien gesamt | $(fmt_int "$TEXT_FILES") |
| Textzeilen gesamt | $(fmt_int "$TOTAL_NONEMPTY") (nicht leer, inkl. Massendaten) |
| Binärmedien | $(fmt_int "$BINARY_FILES") $(plural_files "$BINARY_FILES") |
| Dateien gesamt | $(fmt_int "$TOTAL_FILES") |

*„Aktiver Text“ = MCS + Fremdcode + Dokumentation + Konfiguration. Massendaten (z. B. große ID-Tabellen) werden separat gezählt, damit sie die Prozentwerte nicht verzerren.*

## Sprachverteilung

### Eigencode vs. Fremdcode

| Sprache | Typ | Dateien | Zeilen | Anteil am Code |
|---|---|---:|---:|---:|
EOF
lang_rows code "$CODE_TOTAL"
cat <<EOF

*Dateien mit Mischcode (MCS + Fremdsprache) zählen in beiden Zeilen.*

Nicht als Fremdcode gefunden (0 Zeilen): $(missing_code_langs)

### Dokumentation und Konfiguration

| Sprache | Typ | Dateien | Zeilen | Anteil am Doku-/Konfig-Text |
|---|---|---:|---:|---:|
EOF
lang_rows text "$NONCODE_TEXT"
cat <<EOF

### Massendaten (nicht in Prozenten)

| Sprache | Typ | Dateien | Zeilen | Anteil an Massendaten |
|---|---|---:|---:|---:|
EOF
if (( DATA_LINES == 0 )); then printf '| – | – | – | – | – |\n'; else lang_rows data "$DATA_LINES"; fi
cat <<EOF

## MCS-Dateien

| Datei | Rahmen | MCS-Zeilen | Abdeckung | Typ |
|---|---:|---:|---:|---|
EOF
if (( MCS_FILES == 0 )); then printf '| – | – | – | – | keine |\n'; else mcs_rows; fi
cat <<EOF

## Syntaxhinweise

EOF
warning_rows
cat <<EOF

---

*Diese Datei wird bei jedem Push automatisch neu erzeugt. Nicht manuell bearbeiten.*
*Der MCS-Anteil am Code = MCS-Zeilen / (MCS-Zeilen + Fremdcode-Zeilen).*
EOF
}

build_svg() {
  local color="#3fb950" barw=0 status="$MCS_STATUS" pct
  (( MCS_FILES == 0 )) && color="#f85149"
  barw=$(( 504 * MCS_CODE_X10 / 1000 ))
  pct="$(pct_human "$MCS_CODE_X10")"
  cat <<EOF
<svg xmlns="http://www.w3.org/2000/svg" width="560" height="170" viewBox="0 0 560 170" role="img" aria-label="MCS-Status: $status">
  <rect width="560" height="170" rx="12" fill="#0d1117" stroke="#30363d"/>
  <text x="28" y="32" fill="#8b949e" font-family="monospace" font-size="13">PYLOVARA // MCS-STATUS</text>
  <text x="532" y="32" fill="#8b949e" font-family="monospace" font-size="12" text-anchor="end">MCS 7.5</text>
  <text x="28" y="68" fill="$color" font-family="monospace" font-size="24" font-weight="bold">MCS $status</text>
  <text x="532" y="68" fill="#e6edf3" font-family="monospace" font-size="24" text-anchor="end">$pct %</text>
  <rect x="28" y="86" width="504" height="14" rx="7" fill="#21262d"/>
  <rect x="28" y="86" width="$barw" height="14" rx="7" fill="$color"/>
  <text x="28" y="124" fill="#8b949e" font-family="monospace" font-size="12">$(fmt_int "$MCS_LINES") MCS-Zeilen · $(fmt_int "$MCS_FRAMES_TOTAL") Rahmen · $(fmt_int "$MCS_FILES") Dateien</text>
  <text x="28" y="144" fill="#8b949e" font-family="monospace" font-size="12">Fremdcode: $(fmt_int "$FOREIGN_LINES") Zeilen · Doku: $(fmt_int "$DOC_LINES") Zeilen</text>
  <text x="532" y="144" fill="#484f58" font-family="monospace" font-size="10" text-anchor="end">$(xml_escape "$SOURCE_DATE")</text>
</svg>
EOF
}

# -----------------------------------------------------------------------------
# 12) DATEIEN SCHREIBEN / PRÜFEN
# -----------------------------------------------------------------------------
emit_file() {
  local rel="$1" content="$2" dst="$OUT_DIR/$1"
  mkdir -p "$(dirname "$dst")"
  printf '%s\n' "$content" > "$dst"
}

update_marked() {
  local rel="$1" content="$2" base="${3:-}"
  local src="$REPO_ROOT/$rel" dst="$OUT_DIR/$rel"
  local tmpbase="" block tmpout src_mode=""
  mkdir -p "$(dirname "$dst")"
  if [[ ! -f "$src" && -n "$base" ]]; then
    tmpbase="$TMP_DIR/base.$$.$RANDOM"
    printf '%s\n' "$base" > "$tmpbase"
    src="$tmpbase"
  fi
  [[ -f "$src" ]] && src_mode="$(stat -c '%a' "$src" 2>/dev/null || true)"
  block="$TMP_DIR/block.$$.$RANDOM"
  printf '%s\n' "$content" > "$block"
  tmpout="$TMP_DIR/marked.$$.$RANDOM"
  if [[ -f "$src" ]] && grep -qF "$MARK_START" "$src" && grep -qF "$MARK_END" "$src"; then
    awk -v start="$MARK_START" -v end="$MARK_END" -v block="$block" '
      BEGIN { while ((getline line < block) > 0) text = text line "\n"; close(block) }
      $0 == start { print; printf "%s", text; inblock = 1; next }
      $0 == end { inblock = 0; print; next }
      !inblock { print }
    ' "$src" > "$tmpout"
  else
    if [[ -f "$src" ]]; then cat "$src" > "$tmpout"; else : > "$tmpout"; fi
    printf '\n%s\n%s\n%s\n' "$MARK_START" "$content" "$MARK_END" >> "$tmpout"
  fi
  mv -f "$tmpout" "$dst"
  [[ -n "$src_mode" ]] && chmod "$src_mode" "$dst" 2>/dev/null || true
}

check_one() {
  local rel="$1" a="$OUT_DIR/$1" b="$REPO_ROOT/$1"
  if [[ ! -f "$b" ]]; then printf '[!] fehlt: %s\n' "$rel"; return 1; fi
  if ! cmp -s "$a" "$b"; then printf '[!] veraltet: %s\n' "$rel"; return 1; fi
  return 0
}

README_BASE='# PYLOVARA-SYSTEM

**TWS | MCS** — The Wabe Nest System / Maschinen-Code-Speech.

![MCS-Status](MCS-STATUS.svg)

<!-- MCS-STATS:START -->
<!-- MCS-STATS:END -->

## Dokumentation

- [README.WIKI-NOTES](README.WIKI-NOTES) — System-, Lizenz- und Installationshinweise
- [MCS-STATUS.md](MCS-STATUS.md) — vollständige, automatisch erzeugte MCS-Metrik
- [HANDBÜCHER](HANDBÜCHER) — Lexika und Handbücher

> Die Statusanzeige wird bei jedem Push automatisch durch `.mcs_analyzer.sh` aktualisiert.
> Die GitHub-Sprachleiste ist unabhängig davon; MCS wird hier inhaltlich am Transaktionsrahmen gemessen.'

STATUS_CONTENT="$(build_status)"
REPORT_CONTENT="$(build_report)"
WIKI_CONTENT="$(build_wiki)"
README_CONTENT="$(build_readme)"
SVG_CONTENT="$(build_svg)"

emit_file "$STATUS_FILE" "$STATUS_CONTENT"
emit_file "$REPORT_FILE" "$REPORT_CONTENT"
emit_file "$BADGE_FILE" "$SVG_CONTENT"
if (( ! NO_README )); then
  update_marked "README.md" "$README_CONTENT" "$README_BASE"
  update_marked "README.WIKI-NOTES" "$WIKI_CONTENT"
fi

if [[ -n "${GITHUB_STEP_SUMMARY:-}" ]]; then
  printf '%s\n' "$REPORT_CONTENT" >> "$GITHUB_STEP_SUMMARY"
fi

if [[ "$MODE" == "check" ]]; then
  rc=0
  for f in "$STATUS_FILE" "$REPORT_FILE" "$BADGE_FILE"; do check_one "$f" || rc=1; done
  if (( ! NO_README )); then
    check_one "README.md" || rc=1
    check_one "README.WIKI-NOTES" || rc=1
  fi
  if (( rc == 0 )); then
    printf '[✓] MCS-Status ist aktuell.\n'
  else
    printf '[!] MCS-Status ist veraltet. Bitte .mcs_analyzer.sh ausführen.\n'
  fi
  exit "$rc"
else
  printf '[✓] Status geschrieben: %s, %s, %s\n' "$STATUS_FILE" "$REPORT_FILE" "$BADGE_FILE"
  (( NO_README )) || printf '[✓] README-Blöcke aktualisiert.\n'
fi
