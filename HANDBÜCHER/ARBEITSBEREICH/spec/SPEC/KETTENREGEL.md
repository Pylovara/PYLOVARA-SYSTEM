# Die Kettenregel — wie die Richtung ins Programm kommt

Quelle: `HANDBÜCHER/ARBEITSBEREICH/updates-00-83.dev-notes`, ab Zeile 137
(„NEU | PIPELINE | DATENSTROM ANGABEN") und das Beispiel ab Zeile 297.
Ergänzend `MCSFramework/docs/trennbefehle.syntax-notes` (MCS 2.1).

## Der Grund, warum die Richtung fehlt

In `updates-00-83` stehen **vier Modi**:

```
 MODUS = 0 = GOLDSTANDART    = PENIBLE SYSTEME      mit Datenfluss-Spuren
 MODUS = 1 = OHNE DATENFLUSS = NORMALE ENTWICKLER   ohne Spuren
 MODUS = 2 = FUNKTION ABRUF  = BEDIENT AN DER BIBLIOTHEK
 MODUS = 3 = EINFACH PRINT   = EINZEILER
```

Die alte MCS-Bibliothek ist **Modus 1**. Sie enthält keine Richtungen, weil
Modus 1 keine vorsieht — nicht, weil sie vergessen wurden. Die Richtung ist
im Modus 0 vorhanden, und dort steht sie **als Spalte vor jeder Zeile**.

## 1. Die Spur

Jede Zeile trägt ihre Richtung als Präfix, eingerückt wie ein Zeitdiagramm:

```
 ↑:↓          ↑:↓    ↑:↓    —:—@· »—:—[Ħ: EFI_SIMPLE_TEXT_INPUT_PROTOCOL.ReadKeyStroke() :Ħ]«
 ↑:↓          ↑:↓    ↑:↓           ↑:↓
 ↑:↓          ↑:↓    ↑:↓           —:—@·¤ »—:—[ * ]«
```

Die Anzahl der Präfixe ist die Tiefe der Verschachtelung. Jedes Präfix ist ein
gültiger Pipeline-Token nach `SPEC/PIPELINE.md` (`H U R L X`, zwei Ströme links
und rechts vom Anker).

## 2. Der Kanal wird per `::` gewählt

Das ist die eigentliche Bindung. Dieselbe Anweisung, zwei Schreibweisen:

```
 ÷:: Ø—(<NR>): <KOPIE> :Ø        linker Kanal
 ::÷ Ø—(<NR>): <KOPIE> :Ø        rechter Kanal

 ±:: Ø: <KOPIE> :(<NR>)—Ø
 ::± Ø: <KOPIE> :(<NR>)—Ø
```

**`::` vor dem Symbol = rechter Strom, `::` nach dem Symbol = linker Strom.**
Die Note führt beide Formen ausdrücklich als „KOMPAKTIBEL MIT 1 UND 2 KANAL".

## 3. Sentiatoren — dieselbe Regel

```
 ¶::   /  ::¶     PROZESS-WEITER
 ¶¶:   /  :¶¶     PROZESS-FEHLER
 Ω::   /  ::Ω     WENN-KANN
 ΩΩ:   /  :ΩΩ     WENN-NICHT
```

## 4. Der Steckschlüssel

```
 ****  = STECKSCHLÜSSEL
```

Der Einrastpunkt für weitere Datenströme — in `006-PIPELINE` als „4. Position"
beschrieben.

## 5. Pipeline-Skins

Alle gleichwertig, nur andere Ankerform:

```
 —:—   PIPELINE STANDART
 —©—  —®—  —ĸ—  —#—  —¡—  —!—  —)—     OPTIONAL
```

Farbvorschlag derselben Note: rot = Funktionsabrufe, orange = mathematische,
grün = mechanische. **Skins ändern die Funktion nicht.**

## 6. Die Marker sind kompositionell

Aus der Tastatur-Ebene derselben Note:

```
 @    = REX
 ·    = KERN-1      ·· = KERN-2     ··· = KERN-3     ···· = KERN-4
 ¤    = INTERN      ¤¤ = EXTERN
 ¶    = PROZESS-WEITER           ¶¶ = PROZESS-FEHLER
 Ω    = WENN-KANN                ΩΩ = WENN-NICHT
 £<NR> = RETURN MEHRFACH VERKABELUNG
 ¿<NR> = PARALLELISIERUNGS-PRIORISIERER
```

Damit liest sich `—:—@·¤¤` als: Connector, REX, KERN-1, EXTERN.

## 7. Die Kette selbst

Die Reihenfolge kommt aus MCS 2.1. Dort war sie die Reihenfolge im
Proteinpaket (`SPEC/WERDEGANG.md`):

```mcs
»['run'|Firefox|'open'|Search|'search'|"Pylovara Github"|'Enter']«
```

In 7.x ist dieselbe Reihenfolge die **Komposition am Blockende**, und die
Spur sagt, auf welcher Seite des Doppelpunktes sie läuft.

> Die Regel: **was in 2.1 die Position im Paket war, ist in 7.x die Position in
> der Komposition; die Spur sagt, welcher Strom sie trägt.**

## 8. Zwei Ungereimtheiten in der Quelle

In der 2-Kanal-Liste von `updates-00-83` stehen zwei Einträge, die nicht ins
Muster passen:

```
 ←:↓  = S|U        erwartet: L|U
 ↓:↑  = U|O        erwartet: U|H
```

`S` ist keine der fünf Richtungen, `O` auch nicht. Vermutlich Tippfehler für
`L` und `H` — ich habe sie **nicht korrigiert**, sondern nur vermerkt. Die
ältere Fassung in `006-PIPELINE` (GitHub) schreibt an derselben Stelle `LU`
und `UH`, was die Vermutung stützt.
