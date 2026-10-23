# Der Werdegang der Syntax

Quelle: `Alte-Daten-Testbereiche/` (444 Dateien) und `MCSFramework/docs/`.
Vier Generationen, chronologisch.

## 1. MCS 2.1 — Februar 2026

```mcs
¢|
»['echo'|"Hallo Welt"]«
|¢
```

| Element | Form | Rolle |
|---|---|---|
| Transaktion | `¢| … |¢` | Rahmen |
| Protein | `»['cmd'|arg]«` | Aktion + Argumente |
| Proton | `{Name|Wert}` | Parameter, Status, Physik |
| þ | `þ1623512` | Checksumme / Identität |

**Der Trennbefehl `|` ist eine eigene Schicht** — `MCS.Tokenizer.Core`
zerlegt die Sequenz, der Handler führt sie aus. Die Operatoren der Proteine:

```
""     Print
''     Command
¨¨     Blanker Nenner, Richtig-Abgleich
```

Und Proteine können **Ketten** bilden:

```mcs
»['run'|Firefox|'open'|Search|'search'|"Pylovara Github"|'Enter']«
```

Ein Paket, mehrere Schritte, von links nach rechts. **Das ist die Pipeline
in ihrer ältesten Form.**

## 2. MCS-Plus

```mcs
Rechner PLUS
¢!
    [EINGANG|1|PLUS|2]
    [AUSGANG|ERGEBNIS]
!¢
```

Deutsche Wortebene — die MCD-Idee in ausprobierter Form. Immer noch `|` als
Trenner, aber eckige Klammern und `Rechner` statt `»[ ]«`. Dazu Testfälle:

```
test PLUS { operand1 = 5  operand2 = 3  erwarte = 8 }
```

## 3. MCS 7.x — August 2026

```mcs
¢!
»—[¥: ++ :¥]—«««~: BAUMODUS FUNKTION SPEICHER EINGANG :~«
 ¬· »—[$: …/RECHEN-OPERATOR/ADD :$]—«
     ¬· »—[ ++ ]—«
         ÷ Ø—(1): ++ :Ø
 !¢
```

Der Bruch: `|` verschwindet, Klammern werden zu `»—[ ]—«`, Marker bekommen
Doppelpunkte (`¥:`, `Ħ:`, `$:`, `:Ø`), Adressen werden explizit.
Aus Proteinen werden Blöcke, aus Protonen werden IO-Felder.

## 4. TWS 0.3 — Oktober 2026

```mcs
—:—[": ᴹᶜˢ ˢʰᵉˡˡ :"]
```

Die Pipeline mit `H U R L X`, der Doppelpunkt als **funktionsloser Anker**.

## Die Biographie des Doppelpunkts

Das ist der Faden, der durch alle Generationen läuft:

| Generation | Doppelpunkt | Rolle |
|---|---|---|
| MCS 2.1 | vorgeschlagen: `::` für Sequenzen, `\|\|` für parallele Befehle | Idee, nicht gebaut |
| MCS 7.x | `¥:`, `$:`, `Ħ:`, `:Ø` | Feldmarker |
| TWS 0.3 | `↑:↓`, `—:—[` | Anker, ausdrücklich funktionslos |

In `trennbefehle.syntax-notes` wird `::` als Sequenz-Trenner vorgeschlagen.
Gebaut wurde er nie — aber der Doppelpunkt ist geblieben und trägt heute die
Pipeline.

## Was verloren ging

Beim Umbruch von 2.1 auf 7.x fielen drei Dinge weg, die 2.1 schon hatte:

1. **Die Proteinkette.** `'run'|'open'|'search'|'Enter'` — mehrere Schritte
   in einem Paket, in Reihenfolge. In 7.x gibt es dafür nur noch die
   Komposition am Blockende, und die trägt keine Richtung.
2. **Das Proton-Register.** `{Wait-s|2}`, `{Watt|200}`, `{Signed|2048}` — ein
   offenes, erweiterbares Register für Parameter. In 7.x sind IO-Felder
   namenlos und positionsbezogen.
3. **þ im MCS.** In 2.1 trägt jede Transaktion eine Checksumme. In 7.x gibt
   es þ nur noch in den System-IDs.

## Die Folge für die Pipeline-Frage

Die Pipeline ist nicht neu — sie ist die **Wiederkehr der Proteinkette**.
2.1 hatte die Reihenfolge im Paket, 7.x hat sie in die Komposition verlegt,
TWS 0.3 will sie als Richtung (H/U/R/L) zurückholen.

Das erklärt, warum die Bindung nirgends definiert ist: sie war in 2.1
**implizit durch die Paketreihenfolge** und wurde nie als Regel
aufgeschrieben. Als die Reihenfolge in die Komposition wanderte, ging die
Richtung verloren — und niemand hat sie dort vermerkt.
