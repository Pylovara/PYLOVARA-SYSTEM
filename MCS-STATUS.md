# MCS-STATUS

> Automatisch erzeugt durch `.mcs_analyzer.sh` (2.0.0).
> Syntax: `¢!` … `!¢` · Modus: `line` · Verschachtelung: 1
> Quell-Commit: `28a6500d` · Stand: 2026-11-08 14:54:15 UTC

## Kurzstatus

**MCS ERKANNT** — 5 Dateien mit gültigen Transaktionsrahmen.

```text
MCS   ███░░░░░░░░░░░░░░░░░░░░░  12,9 % vom Code
Fremd █████████████████████░░░  87,1 % vom Code
```

| Kennzahl | Wert |
|---|---:|
| MCS-Dateien | 5 |
| davon MCS-dominant (≥ 80 %) | 2 |
| davon MCS-gemischt | 3 |
| MCS-Transaktionsrahmen | 65 (64 top-level, 1 verschachtelt) |
| MCS-Zeilen gesamt | 476 |
| – davon Inhalt | 346 |
| – davon Rahmensymbole | 130 |
| – davon in MCS-dominanten Dateien | 413 |
| – davon in gemischten Dateien | 63 |
| MCS-Anteil am aktiven Text | 6,1 % |
| MCS-Anteil am Code | 12,9 % |
| Fremdcode | 3.222 Zeilen in 52 Dateien |
| Dokumentation | 3.540 Zeilen |
| Konfiguration | 582 Zeilen |
| Massendaten (nicht in Prozenten) | 641.533 Zeilen |
| MCS-Spuren (nur Beispiele/Text) | 1 Datei |
| Textdateien gesamt | 127 |
| Textzeilen gesamt | 649.353 (nicht leer, inkl. Massendaten) |
| Binärmedien | 73 Dateien |
| Dateien gesamt | 200 |

*„Aktiver Text“ = MCS + Fremdcode + Dokumentation + Konfiguration. Massendaten (z. B. große ID-Tabellen) werden separat gezählt, damit sie die Prozentwerte nicht verzerren.*

## Sprachverteilung

### Eigencode vs. Fremdcode

| Sprache | Typ | Dateien | Zeilen | Anteil am Code |
|---|---|---:|---:|---:|
| Bash | code | 48 | 2.756 | 74,5 % |
| MCS | mcs | 5 | 476 | 12,9 % |
| Bash/Shell | code | 3 | 436 | 11,8 % |
| Lua | code | 1 | 30 | 0,8 % |

*Dateien mit Mischcode (MCS + Fremdsprache) zählen in beiden Zeilen.*

Nicht als Fremdcode gefunden (0 Zeilen): Zsh, Fish, Korn-Shell, Dash, AWK, Sed, C, C/C++-Header, C++, C++-Header, Rust, Python, Kotlin, Java, PHP, JavaScript, TypeScript, Go, Ruby, Perl, C#, Swift, Scala, Dart, R, Julia, Haskell, Clojure, ClojureScript, Elixir, Erlang, F#, Visual Basic, Pascal, Assembler, SQL, GraphQL, Vue, Svelte, Astro, HTML, XHTML, CSS, SCSS, Sass, Less, PowerShell, Batch, Verilog/V, VHDL, Objective-C/MATLAB

### Dokumentation und Konfiguration

| Sprache | Typ | Dateien | Zeilen | Anteil am Doku-/Konfig-Text |
|---|---|---:|---:|---:|
| TWS-Notizen | doc | 20 | 991 | 24,0 % |
| Kernel-Notizen | doc | 5 | 518 | 12,6 % |
| Dev-Notizen | doc | 3 | 444 | 10,8 % |
| Lizenz | doc | 8 | 405 | 9,8 % |
| DNA-Notizen | doc | 5 | 368 | 8,9 % |
| Vision-Notizen | doc | 2 | 258 | 6,3 % |
| Konfiguration | config | 5 | 208 | 5,0 % |
| MCS-Notizen | doc | 3 | 174 | 4,2 % |
| Git-Attribute | config | 1 | 140 | 3,4 % |
| Lizenz/Proto | doc | 1 | 120 | 2,9 % |
| Install-Notizen | doc | 1 | 106 | 2,6 % |
| Wiki-Notizen | doc | 2 | 83 | 2,0 % |
| PV-Konfiguration | config | 1 | 66 | 1,6 % |
| Git-Ignore | config | 1 | 62 | 1,5 % |
| TOML | config | 1 | 47 | 1,1 % |
| INI | config | 1 | 38 | 0,9 % |
| MCS-Zertifikat | doc | 1 | 28 | 0,7 % |
| Systemd-Unit | config | 1 | 21 | 0,5 % |
| Notizen | doc | 1 | 19 | 0,5 % |
| Sicherheitsnotiz | doc | 1 | 17 | 0,4 % |
| Markdown | doc | 1 | 9 | 0,2 % |

### Massendaten (nicht in Prozenten)

| Sprache | Typ | Dateien | Zeilen | Anteil an Massendaten |
|---|---|---:|---:|---:|
| Hexagramm-/ID-Daten | data | 1 | 635.376 | 99,0 % |
| Braille-/ID-Daten | data | 2 | 6.155 | 1,0 % |
| Platzhalter | data | 6 | 2 | 0,0 % |

## MCS-Dateien

| Datei | Rahmen | MCS-Zeilen | Abdeckung | Typ |
|---|---:|---:|---:|---|
| `ARBEITERBIENEN/CONTROL-BASH/LINUX-SHELL/DATEN/SHELL-PROGRAMME/SPICKZETTEL/SPICKZETTEL.WIKI-NOTES` | 53 | 296 | 99,3 % | dominant |
| `SNAPSHOT-VERSIONEN/MCS-ZERTIFIKATE/HALLO-WELT.MCS-ZERTIFIKAT` | 3 | 117 | 80,7 % | dominant |
| `HANDBÜCHER/MCS-LEXIKON/002-PROTEIN.MCS-NOTES` | 4 | 26 | 33,8 % | gemischt |
| `HANDBÜCHER/MCS-LEXIKON/001-TRANSAKTIONSRAHMEN.MCS-NOTES` | 3 | 22 | 24,2 % | gemischt |
| `HANDBÜCHER/ARBEITSBEREICH/lab/tan.dev-note` | 1 | 15 | 14,2 % | gemischt |

## Syntaxhinweise

- `HANDBÜCHER/MCS-LEXIKON/001-TRANSAKTIONSRAHMEN.MCS-NOTES`: 1 unpaarige `!¢` (Zeile 10)
- `HANDBÜCHER/MCS-LEXIKON/002-PROTEIN.MCS-NOTES`: 3 unpaarige `¢!` (Zeile 30,53,55)
- `HANDBÜCHER/VISION-LEXIKON/001-KOGNITIVES-KÜNSTLICHES-GELDSYSTEM.VISION-NOTES`: `¢!`/`!¢` nur als Textspur/Beispiel, kein vollständiger Transaktionsrahmen.

---

*Diese Datei wird bei jedem Push automatisch neu erzeugt. Nicht manuell bearbeiten.*
*Der MCS-Anteil am Code = MCS-Zeilen / (MCS-Zeilen + Fremdcode-Zeilen).*
