# Adressierung der Operatoren — Zuordnung aus dem Bestand

Quelle: `Handbuch/MCSNotes/40-RECHEN-SYMBOLE.MCS-NOTES`, Zeile 170–193,
gegengeprüft gegen die `$:`-Pfade in `BASIS-FUNKTIONEN/MATHEMATIK/RECHEN-SYMBOLE/`
und gegen die Dateinamen in `COMBI-FUNKTIONEN/MATHEMATIK/RECHEN-ARTEN/`.

## Die Brücke

    +  -  ×  ÷   (Original, Hardware-nah)
    ↓
    ++ -- ×× ÷÷  (MCS-Träger, geschrieben in MCS)
    ↓
    ADD SUB MUL DIV …  (Operator, wählt ISA)
    ↓
    PLUS-RECHNEN …  (Rechen-Art, mappt auf die ALU)
    ↓
    BX-ALU / BX-OPCODE  (Komponente, maschinennah)

Das ist die Schnittstelle, von der du gesprochen hast: wir bauen die Hardware
nicht, also mappt MCS die Original-Operatoren auf die eigenen Träger. Ab
`PLUS-RECHNEN` ist alles in MCS geschrieben.

## Symbol → Operator

| Symbol | Operator | Bedeutung |
|---|---|---|
| `++` | `ADD` | Addition |
| `--` | `SUB` | Subtraktion |
| `××` | `MUL` | Multiplikation |
| `÷÷` | `DIV` | Division, Standard |
| `X÷+` | `IDIV` | Division mit Vorzeichen (x86_64) |
| `X÷-` | `DIV` | Division ohne Vorzeichen (x86_64) |
| `A÷+` | `SDIV` | Division mit Vorzeichen (ARM64) |
| `A÷-` | `UDIV` | Division ohne Vorzeichen (ARM64) |
| `R÷+` | `DIV` | Division mit Vorzeichen (RISC-V) |
| `R÷-` | `DIVU` | Division ohne Vorzeichen (RISC-V) |
| `RR÷+` | `REM` | Rest (RISC-V) |
| `RR÷-` | `REMU` | Rest ohne Vorzeichen (RISC-V) |

## Operator → Rechen-Art je ISA

`ADD`, `SUB`, `MUL` sind in allen drei ISAs vorhanden. Die Division ist der
Grund für die Architekturtrennung — dort heißen die Befehle je ISA anders
(`IDIV` / `SDIV` / `DIV` / `DIVU` / `REM` / `REMU`).

| Operator | 86_64 | ARM64 | RISC-V |
|---|---|---|---|
| `ADD` | `PLUS-RECHNEN` | `PLUS-RECHNEN` | `PLUS-RECHNEN` |
| `SUB` | `MINUS-RECHNEN` | `MINUS-RECHNEN` | `MINUS-RECHNEN` |
| `MUL` | `MAL-RECHNEN` | `MAL-RECHNEN` | `MAL-RECHNEN` |
| `DIV` | `GETEILT-RECHNEN` | `…-MIT-VORZEICHEN` | `…-MIT-VORZEICHEN` |
| `IDIV` | `…-MIT-VORZEICHEN` | `…-MIT-VORZEICHEN` | `…-MIT-VORZEICHEN` |
| `SDIV` | `…-MIT-VORZEICHEN` | `…-MIT-VORZEICHEN` | `…-MIT-VORZEICHEN` |
| `UDIV` | `…-OHNE-VORZEICHEN` | `…-OHNE-VORZEICHEN` | `…-OHNE-VORZEICHEN` |
| `DIVU` | `…-OHNE-VORZEICHEN` | `…-OHNE-VORZEICHEN` | `…-OHNE-VORZEICHEN` |
| `REM` | — | — | `…-MIT-VORZEICHEN-REM` |
| `REMU` | — | — | `…-OHNE-VORZEICHEN-REMU` |

Die beiden Striche bei `REM`/`REMU` sind keine Lücke im Entwurf: nur RISC-V
kennt einen eigenen Rest-Befehl. Für die anderen ISAs gibt es im Bestand
keine Datei — und ich erfinde keine.

## Wie die ISA-Auswahl aussieht

Im Bestand gibt es **kein Vorbild für eine Verzweigung**. Es gibt nur
`BAUMODUS AUSWAHL KOMPONENTE … ERSTELLEN` (siehe `BX-ALU`, `BX-OPCODE`),
und das listet Kandidaten, statt zu prüfen.

Umgesetzt ist es deshalb so: der Operator bindet **alle** vorhandenen
ISA-Pfade auf **einen** Ausgangsplatz. Welcher Kandidat greift, entscheidet
die Bindung — nicht der Operator. Das ist die einzige Lesart, die ohne
erfundene Syntax auskommt.

Wenn `ADD` aktiv prüfen und verzweigen soll, brauche ich dafür ein Vorbild
aus deiner Hand — dafür gibt es im Bestand keins.
