# Die Entwicklungslinie 00-78 → 00-82

840 Seiten in fünf PDFs. Ich habe sie nicht gelesen — ich habe sie **vermessen**:
Schlagworte gezählt, die Abschnitte gezielt aufgesucht, die den Bruch zeigen.
Das ist die verdichtete Form.

## Die fünf Stände

| Stand | Seiten | PNS | Dominanter Begriff |
|---|---:|---|---|
| 00-78 | 166 | 2.6.2 | **WARP** (112 Treffer) |
| 00-79 | 164 | 2.6.2 | WARP (115) |
| 00-80 | 167 | 2.6.2 | **KANAL** (103) / DATENSTROM (44) |
| 00-81 | 187 | 2.6.4 | **WABE** (571) / TWS (45) |
| 00-82 | 157 | 2.6.4 | WABE (792) / PIPELINE (29) / KANAL (350) |

Eine Häufigkeitszählung ist kein Beweis — aber sie zeigt, wo jeweils der
Schwerpunkt lag.

## Die vier Begriffe und was sie ablösten

### 1. WARP (00-78, 00-79)

```
 INFO-ID ¡ WARP
 FUNKTION = DIREKTE ORIGINALE COMPILER BINDUNG TRANSPORTRAHMEN
 WARP-LEGACY-START-MODULKLASSE § WARM-C, WARM-PYTHON, WARM-RUST,
                                  WARM-JSON, WARM-XML, WARM-YAML, WARM-SQL,
                                  WARM-APP, WARM-ASM, WARM-GCC, WARM-CLANG,
                                  WARM-HTML5, WARM-MACOS
```

WARP war die **Brücke zu fremden Sprachen** — ein Transportrahmen, der direkt
an originale Compiler bindet. Das ist heute der Job der **Arbeiterbienen**
(„SCHNITTSTELLEN ZU ISA SPEZIFISCHE PROGRAMM ANSTEUERUNGEN UND HABEN
FREMDSPRACHEN ALS BRÜCKE INCLUDIERT" — aktuelles README).

**Der Begriff hat gewechselt, die Aufgabe nicht.**

### 2. NAVIGATOR / KANAL / DATENSTROM (00-80)

Ein vollständiges Klassensystem:

```
 NAVIGATOR-HAUPTKLASSE § NAVH-DATENSTROM
 SCHALTUNGSBEZEICHNUNG = DATENSTROM
 VAL-KENNUNG = NAD0
 FUNKTION = DATENSTROM | DATENSTRANG VERARBEITUNG

 NAVIGATOR-DATENSTROM-UNTERKLASSE § NAVU-KANAL1 … KANAL8
 NAVIGATOR-DATENSTROM-KANALKLASSE-ANKERKLASSE § NAVAN-DATENSTROM-ENDE
```

**Acht Kanäle**, und es gibt bereits eine **ANKERKLASSE** — der Doppelpunkt
als Anker ist also älter als die Pipeline-Note von gestern. Dazu ein ganzer
Katalog von Auslöserklassen (ALPHA, BAU, KERN, HAUPT, ANSCHLUSS, ANKER, RELAIS,
WERT, ZEIT, NUMMER, LINKER, RAM …).

### 3. WABE / TWS (00-81)

Der Umbruch: statt Kanälen jetzt **Waben** als Struktur. 571 Treffer, TWS
taucht zum ersten Mal auf. Das ist die Geburtsstunde von dem, was heute als
TWS 0.3 im GitHub steht.

### 4. PIPELINE (00-82 und heute)

Aus acht Kanälen werden **zwei**, aus NAVIGATOR wird **PIPELINE**, und die
Steuerung schrumpft auf fünf Richtungen: `H U R L X`. Der Anker bleibt.

## Was das für die Ungenauigkeit bedeutet

Die Pipeline ist die **jüngste** Schicht — sie entsteht in 00-82 und wird erst
im TWS 0.3 von gestern fertig beschrieben. Alle Schichten darunter (WARP,
KANAL, NAVIGATOR) sind beim Umbau auf Waben **weggefallen**, nicht überführt.

Deshalb ist die Bindung Pipeline ↔ MCS ungenau: es gibt keine alte Regel, die
man hätte mitnehmen können. Die KANAL-Klasse aus 00-80 wurde durch die Wabe
ersetzt, nicht in sie übersetzt.

## Was ich nicht gefunden habe

In keiner der fünf Versionen steht eine Regel der Form:

> „eine MCS-Anweisung mit Richtung U verhält sich so …"

Die Pipeline beschreibt in allen Ständen **Notation** (welche Zeichen, welche
Richtung, welcher Kanal). Sie beschreibt nirgends die **Semantik** an der
Grenze zu MCS. Das ist keine Lücke in meinem Verständnis, sondern eine Lücke
im Material — und der Grund, warum mein Prüfer nur Form prüfen kann.
