# Pipeline — das Datenstrom-Modell

Quelle: TWS-LEXIKON `006-PIPELINE` (GitHub, Stand `DO-22-10-2026`) und
Kernel-Note `52-PIPELINE`. Geprüft von `tools/pipeline.py`.

## Die fünf Richtungen

| Kürzel | Bedeutung | Pfeil |
|---|---|---|
| `H` | HOCH | ↑ |
| `U` | RUNTER | ↓ |
| `R` | RECHTS | → |
| `L` | LINKS | ← |
| `X` | NICHTS | (leer) |

`X` ist ein **leeres Feld**, kein Wert — es erlaubt, mit einem Kanal granular
zu arbeiten.

## Der Doppelpunkt

> „WENN WIR EINEN DOPPELPUNKT SEHEN, IST ER FUNKTIONSLOS. ER WIRD AUCH BEI DEM
> BOXIS SYSTEM VERWENDET, UM DEN UMSTAND DER MUSEUMSREIFEN UTF TASTATUREN ZU
> UMGEHEN." — `006-PIPELINE`

Der Doppelpunkt ist ein **Anker zur Symbolerweiterung**, kein Operator. Das
erklärt zwei Dinge, die ich vorher für Syntaxfehler hielt:

- `": <IO> :"` — die Klammer von `BX-PRINT` ist `"` plus Anker, nicht ein
  eigenes Zeichen.
- `—:—[` — die Shell-Form ist `—[` plus Anker.

Der Anker existiert, damit auf einer 1980er-Tastatur eindeutige Befehle
möglich sind, ohne neue Tasten zu erfinden.

## Ein Kanal

Ein Strang, die andere Seite ist `X`:

```
↓:   UX     linker Strom, runter
↑:   HX     linker Strom, hoch
←:   LX     linker Strom, links
→:   RX     linker Strom, rechts
 :↓  XU     rechter Strom, runter
 :↑  XH     rechter Strom, hoch
 :←  XL     rechter Strom, links
 :→  XR     rechter Strom, rechts
```

## Zwei Kanäle

Beide Seiten gefüllt — zwei **unabhängige** Ströme:

```
↑:↑  HH      ↑:↓  HU      ↑:→  HR      ↑:←  HL
→:↑  RH      ←:↑  LH      →:↓  RU      ←:↓  LU
↓:←  UL      ↓:→  UR      ↓:↑  UH      ↓:↓  UU
```

> „DIE DATENSTRÖME SIND LINKS UND RECHTS VOM DOPPELPUNKT ANGEORDNET UND SIND
> ZWEI VERSCHIEDENE STRÖME. […] SIE SCHIEBEN DIE STRÖME NICHT ZUSAMMEN WENN
> SIE NICHT IN DIE RICHTUNG ZEIGEN."

## Die 25 Kombinationen

`tools/pipeline.py` kann alle 25 erzeugen und prüfen. In der Note genannt sind
**20**; es fehlen `RR`, `RL`, `LR`, `LL` und `XX`.

**Beobachtung, keine Regel:** Die vier fehlenden Zweikanal-Fälle sind genau
die, in denen der linke Strom waagerecht zeigt. Waagerecht ist durch die
Seite bereits bestimmt (links ist links, rechts ist rechts); senkrecht muss
ausdrücklich gesagt werden. `XX` wäre schlicht kein Strom. Ob das Absicht ist
oder nur die Liste unvollständig, weiß nur der Urheber — ich habe es nicht
als Prüfregel eingebaut, sondern nur vermerkt.

## Was hier fehlt — und warum das die Ungenauigkeit erklärt

Die Pipeline beschreibt **Richtungen**. In der MCS-Bibliothek, die ich
rebaut habe, steht davon **nichts**. Keine der 110 Dateien sagt, in welche
Richtung ein Datenstrom läuft, welcher Kanal gemeint ist oder wo der Anker
sitzt.

Die Pipeline ist damit die Schicht, die zwischen MCS und TWS vermittelt — und
genau sie ist in den Quellen unfertig:

- `52-PIPELINE.KERNEL-NOTES` ist als `DEVWORKSPACE` markiert, mit leeren
  `LINK #`-Zeilen und `AXIOM ¡` ohne rechte Seite.
- `006-PIPELINE.TWS-NOTES` beschreibt die **Notation** vollständig, aber nicht
  die **Bindung** an eine MCS-Anweisung.

Solange diese Bindung fehlt, kann kein Werkzeug prüfen, ob ein MCS-Programm
richtig verschaltet ist. Es kann nur prüfen, ob es formal sauber ist — und das
tut es: 142/142 Altdateien, 111 Dateien ohne IO-Befund, 0 Formfehler.
