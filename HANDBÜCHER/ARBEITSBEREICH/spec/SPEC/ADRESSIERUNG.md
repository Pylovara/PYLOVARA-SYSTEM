# Adressierung — was belegt ist und was offen ist

Stand: 23.10.2026 · Quelle: alte MCS-Bibliothek + `ALTER BEREICH_system-ids`

## 1. Belegt

**Regel (aus `002-WABENAUFBAU.WABEN-NOTES`):**
> „2. EINMAL MIT BRILLIANS 8 PATTER **2 PUNKT VIERER BLÖCKEN**"

Daraus folgt eindeutig:

| Eigenschaft | Wert |
|---|---|
| Zellen pro ID | 4 (4x1) |
| Punkte pro Zelle | genau 2 |
| Zustände pro Zelle | C(8,2) = 28 |
| Adressraum | 28⁴ = **614.656** |

**Gegenprobe:** `ALTER BEREICH_system-ids` enthält 490 IDs. Alle 490 haben exakt
4 Zellen mit je genau 2 Punkten. Keine Ausnahme. Die Regel ist damit nicht nur
behauptet, sondern am Bestand messbar.

**Beispiel:** `⠆⢁⡄⢐` = Punkte (2,3)(1,8)(3,7)(5,8) — vier Zwei-Punkt-Zellen.

## 2. Befund: die alte Bibliothek hat keine IDs

181 Dateien, 111 KB. Ergebnis der Zählung:

- 4x1-IDs in der MCS-Bibliothek: **0**
- 3-Zellen-Adressen (Altbestand): **286 verschiedene**

Die Bibliothek adressiert also lokal mit 3 Zellen (`⠀⠽⢐` = `00 3D 90`), nicht
mit dem 4x1-System. Die System-IDs existieren bisher nur im Register
`ALTER BEREICH_system-ids`, nicht in den Programmdateien.

## 3. Befund: die Symboladressen sind instabil

Jede Datei vergibt ihren Symbols eine **eigene** Adresse:

| Symbol | verschiedene Adressen |
|---|---:|
| `R1` | 12 |
| `R2` | 12 |
| `ALU` | 18 |

Gleichzeitig: **keine** Adresse ist doppelt belegt. Es gibt also keine Kollision,
aber auch keine Stabilität — `R1` bedeutet in jeder Datei etwas anderes.

Das ist der eigentliche Grund, warum ein 1:1-Kopieren nichts bringt: die Kopie
hätte denselben Fehler. Der Neubau braucht ein **globales Register**: ein Symbol,
eine Adresse, für immer.

## 4. Offen — zwei Fragen, die nur der Urheber beantworten kann

### 4.1 Zählordnung der 28 Muster

Eine ID in eine Zahl (`1 2 3 4 5 …`) zu übersetzen, braucht eine feste Reihenfolge
der 28 Zwei-Punkt-Muster. Im Bestand finden sich **zwei** Kandidaten:

**(a) lexikographisch** — erstes Punkt bleibt, zweites zählt hoch:

```
(1,2) (1,3) (1,4) (1,5) (1,6) (1,7) (1,8) (2,3) …
⠃     ⠅     ⠉     ⠑     ⠡     ⠣     ⠁     ⠆
```
Belegt in der BION-Gruppe: `⠃ ⠅ ⠉` = laufende Nummern.

**(b) spaltenweise** — zweites Punkt bleibt, erstes zählt hoch:

```
(1,2) (1,3) (2,3) (1,4) (2,4) (3,4) … (5,8) (6,8) (7,8)
                                          ⢐     ⢠     ⣀
```
Belegt in der HAUPTWABE-Gruppe: `⢐ ⢠ ⣀` = laufende Nummern.

Beide Ordnungen kommen im Bestand vor. `tools/braille.py` kann beide
(`--ordnung lex|spalte`), Standard ist derzeit `lex`.

### 4.2 Ist das ALTER BEREICH eingefroren?

Sind die 490 dort vergebenen IDs unveränderlich (dann hänge ich neue nur an),
oder ist die Datei ein Arbeitsblatt, das neu geordnet werden darf?

## 5. Vorschlag für den Neubau

1. **Register als Single Source of Truth:** `register/ids.json` — jede ID genau
   einmal, mit Rolle, Name, Herkunft, Datum. Aus `ALTER BEREICH` importiert,
   danach nur noch fortgeschrieben.
2. **Generator statt Handarbeit:** `tools/build.py` erzeugt jede Bibliotheksdatei
   aus einer Spezifikation. Adressen werden vergeben, nicht getippt.
3. **Prüfer:** `tools/check.py` — jede ID eindeutig, jede Zelle 2 Punkte,
   jeder Aufruf zeigt auf ein existierendes Symbol, jede Datei schließt mit `!¢`.
4. **Reihenfolge:** erst ein vertikaler Schnitt (TASTATUR → RECHEN-SYMBOLE →
   RECHEN-ARTEN → WÖRTERBUCH), dann BOXIS, dann der Rest. Nach jedem Schnitt:
   Prüfer grün, Commit.

## 6. Nachtrag — Korrektur meines eigenen Befunds

Abschnitt 3 war teilweise falsch gelesen. Ich hatte gezählt: „`R1` hat 12
verschiedene Adressen". Beim genaueren Hinsehen steht in den Dateien:

```
 ¬· »—[¥: R1 :¥]—«
     ÷ Ø—(ADRESSE): ** :Ø
```

`R1` ist nur die **Beschriftung** des Blocks. Der eigentliche Platz ist `**`,
und `**` ist ein anonymer Platzhalter — pro Datei neu, und das ist richtig so.
Aus demselben Grund war `×:  :×` „instabil": es ist ein Platz, kein Symbol.

Was davon übrig bleibt, ist echt: die **benannten Symbole** (`++`, `--`, `××`,
`÷÷`, `R÷+` …) wurden in verschiedenen Dateien an verschiedenen Adressen
gebunden. Im Neubau gilt jetzt:

| Was | Adresse |
|---|---|
| benanntes Symbol (`++`, `R÷+`, `PLUS-RECHNEN` …) | **eine** Adresse, über alle Dateien |
| Platzhalter (`*`, `**`, `×:  :×`, `Ħ:  :Ħ`) | eigene Adresse je Datei |

Das ist eine Entscheidung, keine Messung. Die Alternative wäre, jeden Aufruf
als eigenen Datenslot zu behandeln — dann teilen sich zwei Dateien, die `++`
rufen, **nicht** denselben Platz. Wenn das gewünscht ist, ist es eine Zeile
in `tools/registry.py`.
