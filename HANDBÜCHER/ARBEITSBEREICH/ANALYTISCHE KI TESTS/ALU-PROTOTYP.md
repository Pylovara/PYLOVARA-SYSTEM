# Der ALU-Prototyp — gemessen, nicht vermutet

Quelle: `Alte-Daten-Testbereiche/MCSKernel0.2/Logic/mcs_alu_rechner.c`
Vorgehen: kompiliert und ausgeführt, nicht nur gelesen.

## Was der Prototyp tut

```c
long mcs_alu_berechne(const char *ausdruck)   // Inhalt einer Boxi [× … ×]
```

Er nimmt den **Inhalt der Boxi als Zeichenkette** und rechnet darauf. Das ist
die Antwort auf die Frage, wie die ALU ihre Eingänge bekommt: nicht als drei
Adressen, sondern als **Ausdruck**.

Referenzen stehen als `(n)` im Ausdruck. In den MCS-Testdateien sind sie
zusätzlich von Platzhalter-Markern umschlossen:

```
»[×¨(1)¨ > ¨(2)¨×]        alufix1
»[×°(1)° > °(2)°×]        alufix2
```

Beides ist dasselbe: ein Ausdruck mit Feed-Referenzen.

## Was gemessen herauskam

| Eingabe | Ausgabe | richtig wäre |
|---|---|---|
| `5 + 3` | **53** | 8 |
| `5 - 3` | **53** | 2 |
| `15 / 3` | **153** | 5 |
| `5 * 3` | 15 | 15 ✓ |
| `3 > 2` | 1 | 1 ✓ |
| `(1) + 500` | **1500** | 507 (bei Feed1 = 7) |

## Die Ursache

`mcs_alu_get_val` tut genau das, was sein Kommentar sagt:

```c
// Wir nehmen NUR Ziffern. Alles andere (¨, Leerzeichen) fliegt raus.
```

Daraus folgen zwei Fehler in einem Schritt:

1. `5 + 3` → die Ziffern bleiben stehen → `53`.
2. `(1)` → die Ziffer bleibt stehen → die **Zahl 1**, nicht der **Wert von
   Feed 1**.

Der zweite Fehler ist der tödliche. `¨(1)¨ > ¨(2)¨` vergleicht immer `1 > 2`
und liefert immer 0 — unabhängig davon, was in den Feeds steht. Deshalb
existieren `mcs_alufix1.mcs` und `mcs_alufix2.mcs`, und deshalb steht in
alufix2 wörtlich:

```
-·· »['echo "Nope, immer noch kaputt"']«
```

Es war nicht kaputt, weil die Idee falsch war. Es war kaputt, weil eine
Hilfsfunktion die Klammer nicht als Referenz gelesen hat.

## Die Korrektur

`tools/alu_referenz.py` löst Referenzen über den Feed-Speicher auf und rechnet
dann. Selbsttest und die vier Testfälle aus `01.operatoren.mcsplus-test`:

```
✓ PLUS       5 + 3 = 8
✓ MINUS      5 - 3 = 2
✓ MAL        5 * 3 = 15
✓ GETEILT    15 / 3 = 5
4 bestanden, 0 fehlgeschlagen
```

## Was das für die MCS-7.5-Kette bedeutet

Der Ausdruck in MCS 7.5 lautet `×: * ++ * :×` — Feld, Operator, Feld. Der
Ausdruck in MCS-Plus lautet `×¨(1)¨ > ¨(2)¨×` — Wert, Operator, Wert.

**Es ist dieselbe Form.** Die IO-Klammer der ALU trägt einen Ausdruck, dessen
drei Felder Operand, Operator und Operand sind. Meine n+1-Regel aus
`SPEC/HONIGTROPFEN-VERTRAG.md` beschreibt genau das, und beide Schreibweisen
sind nur verschiedene Marker für dieselbe Sache.
