# Komponenten-Beschreibung — Kernel 806

Quelle: `kernel-nr: 806`, Stand `[03:45:12 | So 24 Mai 2026]`, `STATUS : UNFERTIG`.
Älter als die TWS-0.3-Reihe. Teilweise leer. Hier steht nur, was drinsteht.

## Der Katalog

| Nr | Komponente | Symbol | Kern der Aussage |
|---|---|---|---|
| 02 | Transaktionsrahmen | `¢!` … `!¢` | Erkennungsrahmen für MCS |
| 03 | **Protein** | `—[]` / `[]` | ON-PIPE trägt die Weiterführung (`—`), OFF-PIPE trägt Daten ohne Logik |
| 04 | **Proton** | `{}` | Hardware-Versorgung: `{<NAME><BINK><WERT>}` |
| 05 | **Trennbefehl** | `\|` | Trennwand zwischen BOXI und BOXI bzw. BOXI und Proton |
| 06 | **Bindungsbefehl** | `;` | bindet im Proton den Hardware-Namen an den Wert: `{<NAME>;<WERT>}` |
| 07 | **Aktionsdraht** | `»«` | Ausführungsrecht: ausführbar / nicht ausführbar |
| 08 | **BOXI** | siehe unten | Ansteuerung der Systemebenen |
| 09 | Master | — | Inhaltsverzeichnis, systemweite Suchanfrage |
| 10 | **REX** | `¬` | Anschluss an ein ON-PIPE-Protein, bis zu 4 Weitergaben |
| 11 | **Argument** | `««` | Modifikator am Aktionsdraht |
| 13 | **Sentiator** | `¶` / `¶¶` | Reflex-Entscheidung, stack-less, 1 oder 0 |
| 23 | **NITROX** | `±` / `÷` | Cache-Takt der ALU |
| 12, 14–22, 24–34 | — | **leer, „IN ARBEIT NACH URLAUB"** |

## BOXI — die elf Türen

Der alte Stand schreibt sie **mit Leerzeichen**, nicht mit Doppelpunkt:

```
 BOXI-PRINT   => " <IO> "        BOXI-MCSCMD      => ~ <IO> ~
 BOXI-ALU     => × <IO> ×        BOXI-MASCHINENCODE => º <OPCODE-LISTE> º
 BOXI-SYSCALL => , <IO> ,        BOXI-MASCHINENCODE => º <MASCHINENBEFEHL> º
 BOXI-ABI     => Ħ <IO> Ħ
 BOXI-SHELL   => ' <IO> '
```

> „BOXIS WERDEN IN PROTEIN TRÄGER PER ZEICHEN GESETZ UND HABEN ZUM PROTEIN
> RAHMEN SELBST KEINEN ABSTAND, WÄHRENDESSEN SIE ZUM INHALT WIEDERUM EIN
> LEERZEICHEN HABEN."

Der Doppelpunkt in `×: <IO> :×` (TWS 0.3) ist also der später eingeführte
**Anker** — die ältere und eigentliche Form ist das Leerzeichen.

## Die REX-Flächen

```
 REX              => ¬
 REX-FLÄCHEN      => ·  bis  ····
 REX-KOORDINATOR  => ¤
 REX-ALU-RÜCKWERT => =
 REX-STATUS       => :
```

Damit ist `¬·` kein Blockzeichen, sondern **REX auf Fläche 1**. Und
`¬·¤¤` heißt: REX, Fläche 1, Koordinator, extern. Die Marker der
MCS-Bibliothek sind also **Schaltungsangaben**, keine Einrückung.

## ÷ und ± sind keine Zuweisung und kein Abruf

Das ist die wichtigste Korrektur an meinem Modell:

```
 ± = FOOD-NITROX : greift Daten ab und speichert sie im Cache  (FF = FÜHRENDER FEED)
 ÷ = KOPF-NITROX : speist Daten aus dem Cache in den Takt       (FT = TRAGENDER FEED)
```

Ich hatte `÷` als „Zuweisung" und `±` als „Abruf" gelesen. Das beschreibt
zufällig die Wirkung, aber nicht die Sache: es sind die **zwei Richtungen des
Feed-Cache-Takts** — nehmen und geben. Das erklärt auch, warum sie immer
paarweise auftreten.

## Die Sentiator-Semantik

```
 ¶   WENNKANN    Signal wird ABGELENKT bei FREIGABE
 ¶¶  WENNNICHT   Signal wird UMGELEITET bei BLOCKADE
```

> „SIE ANALYSIEREN NICHT DEN GRUND, SIE LÖSEN AUS."

Stack-less, keine RAM-Allokation, Rückgabe 1 oder 0. Das ist die Schicht, an
der AIMS später Ethik in Ablauf übersetzt.

## Der unklare Bereich (Seite als „MUSS GEHÄRTET WERDEN" markiert)

| Komponente | Symbol | Aussage |
|---|---|---|
| FEED (FF/FT) | — | Synchronisationsvektor der NITROX-Ströme, gleicher Nanosekunden-Takt |
| VATRON | `©` | Firewall-Gatter, Bindungs-Wächter, kappt Ströme bei fehlerhaften Frequenzen |
| NAVIGATOR | `$` / `§` | Adressanker: `$` Userland, `§` interner Kernel-Zugriff |
| SYNAPTOR | `¿¹` / `¬·¤›` | synaptische Brücke, verknüpft parallele Leitungen, gibt Feedback an den Sentiator |
| ZYTRON | `£` | Schleifenmaschine, Rückkopplung ohne CPU-Stack |

## Was das für den Ordner bedeutet

Drei Stellen in meinen Dokumenten sind jetzt präziser:

1. `SPEC/ADRESSIERUNG.md` und `tools/mcs.py` nennen `÷`/`±` „Zuweisung"/„Abruf".
   Richtig ist **Kopf-Nitrox / Food-Nitrox**. Die Prüfungen bleiben gültig, die
   Begriffe nicht.
2. Die IO-Klammer ist älter als der Anker: `× <IO> ×` vor `×: <IO> :×`. Beide
   Formen sind gültig, die zweite ist die neuere Schreibweise.
3. `¬·` ist eine REX-Fläche, keine Einrückung — die Bibliothek ist also
   **verschaltet**, nicht nur gegliedert.
