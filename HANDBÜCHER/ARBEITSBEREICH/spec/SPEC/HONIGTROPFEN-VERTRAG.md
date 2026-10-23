# Der Honigtropfen-Vertrag

Abgeleitet **ausschließlich** aus `09-BOXIS.KERNEL-NOTES`. Kein Entwurf, keine
Erfindung — die Axiome stehen so in der Datei, elfmal identisch.

## Der Ablauf, den die Kernel-Seite vorgibt

```
 # AXIOM | VORBAU
 AXIOM ¡ <START> ⧖⧖ ⧓ <EINGABE> Ξ <PRODUKT>
 PRODUKT <ID> ': <IO>

 # AXIOM | RAM
 AXIOM ¡ <PRODUKT> ⍠ <RAM>

 # AXIOM | (EINGABE COPIE)
 AXIOM ¡ <EINGABE> Ξ <EINGABEHALTEN>
 EINGABEHALTEN <ID> <IO>

 # AXIOM | (EINGABE ABGLEICHEN)
 AXIOM ¡ <EINGABEHALTEN> ⟗ ⧖⧖ <HONIGTROPFEN> Ξ <AUSGABE>
 AXIOM ¡ <AUSGABE> ⧑ <AUSGABE-LINK>
 AUSGABE <ID> ': <IO>

 # AXIOM | EINGABEBEHALTEN LÖSCHEN
 AXIOM ¡ <EINGABEHALTEN> ⎚

 # AXIOM | RAM
 AXIOM ¡ <AUSGABE> ⍠ <AUSGABE-EINHEIT>
 EINHEIT <ID> ': <IO>
```

Übersetzt:

1. `⧖⧖ ⧓` — **warten**, dann Start und Eingabe **verbinden** → Produkt.
2. `⍠` — Produkt **in den RAM laden**.
3. `Ξ` — Eingabe in `EINGABEHALTEN` **kopieren** (das Original bleibt).
4. `⟗ ⧖⧖` — `EINGABEHALTEN` **gegen den Honigtropfen abgleichen**, dabei warten,
   Ergebnis ist `AUSGABE`. Das ist der Punkt, an dem die Assembly läuft.
5. `⧑` — Ausgabe **weitergeben**.
6. `⎚` — `EINGABEHALTEN` **löschen**.
7. `⍠` — Ausgabe **als Einheit ablegen**.

## Was daraus für den Payload folgt

Ein Honigtropfen-Payload muss:

| # | Anforderung | Beleg |
|---|---|---|
| 1 | den Inhalt von `EINGABEHALTEN` lesen | Schritt 4 |
| 2 | ihn gegen seine `<ART>` **abgleichen** (Name/Vergleich) | `⟗` = NAME ABGLEICHEN |
| 3 | **synchron** antworten — der Kernel wartet (`⧖⧖`) | Schritt 4 |
| 4 | genau eine `AUSGABE` erzeugen | Schritt 4 |
| 5 | **keinen Zustand behalten** — der Eingabeinhalt wird danach gelöscht | Schritt 6 |

Punkt 5 ist der wichtigste: der Honigtropfen ist **zustandslos**. Er bekommt
einen Eingabeinhalt, gleicht ihn ab, gibt etwas zurück, fertig. Alles, was
erinnert werden muss, gehört in die Waben, nicht in die Assembly.

## Was der Vertrag noch **nicht** festlegt

Das ist der Grund, warum ich keine elf Dateien angelegt habe. Es fehlt:

1. **Das Binärformat von `<IO>`.** Woraus besteht ein Eingabeinhalt? Länge,
   Kopffeld, Namensfeld, Nutzdaten? Ohne das kann niemand einen Payload
   schreiben, der den Abgleich besteht.
2. **Die `<ART>`-Vokabeln.** In der Note steht überall nur `<ART>` als
   Platzhalter. Gibt es eine feste Liste (z. B. `IO`, `CORE-ART`, `AUSGABE` —
   diese drei kommen in anderen Kernel-Noten vor), und wer vergibt sie?
3. **Die Aufrufkonvention.** In welchem Register / an welcher Adresse erwartet
   die Assembly den Zeiger auf `EINGABEHALTEN`, und wohin schreibt sie `AUSGABE`?
4. **Der Umfang eines Payloads.** Eine Routine pro Komponente (11), oder eine
   pro `<ART>` (dann teilen sich mehrere Komponenten eine)?

Sobald **1** und **3** beantwortet sind, ist der Payload schreibbar. **2** und
**4** entscheiden nur, wie viele es werden.

## Aufgelöst: die IO-Klammer

`09-BOXIS.MCS-NOTES` nennt elf Klammerpaare — und damit ist `<IO>` kein
Binärformat, sondern **der Text zwischen der Klammer der jeweiligen
Komponente**:

| Klammer | Komponente | | Klammer | Komponente |
|---|---|---|---|---|
| `": <IO> :"` | PRINT | | `~: <IO> :~` | MCD |
| `×: <IO> :×` | ALU | | `$: <IO> :$` | PFAD |
| `,: <IO> :,` | SYSCALL | | `§: <IO> :§` | NET |
| `Ħ: <IO> :Ħ` | ABI | | `º: <IO> :º` | OPCODE |
| `': <IO> :'` | SHELL | | `ºº: <IO> :ºº` | MASCHINENCODE |
| `¥: <IO> :¥` | EINGABE | | | |

Grundregel derselben Seite: **„VOM BOXIS RAND : ZUM ENDE : IST IMMER EIN
LEERZEICHEN ZU VERWENDEN !"** — die Felder sind also durch Leerzeichen
getrennt und positionsbezogen.

Daraus folgt die Adressregel, die ich im Bestand gefunden und geprüft habe:

```
Ausgabeblock            »[×: * ++ * :×]«          3 Felder
Komposition             »[(3 (1) (1013) (2))]«    4 Adressen
```

**Die Komposition führt immer die Komponente selbst und danach jedes Feld
in Reihenfolge — also *n+1* Adressen bei *n* Feldern.**

Geprüft mit `tools/io_pruefung.py`: 111 Dateien (Bibliothek + Zertifikat),
**0 Befunde**. Die Regel ist damit nicht nur behauptet, sondern am Bestand
messbar — so wie die 4×2-Punkte-Regel der System-IDs.

## Was jetzt noch offen ist — nur noch ein Punkt

Die Frage ist nicht mehr „wie übergibt MCS die Daten". Das ist beantwortet.
Offen ist nur noch die **untere Kante**:

> Wie kommt der Inhalt der IO-Klammer physisch in die Assembly?

Also die ABI-Ebene: Register oder Speicherbereich, Wer-zu-Wer, und wer
übersetzt die Feldadressen in Zeiger. Das ist `BX-ABI` (`Ħ: <IO> :Ħ`) und
`BX-MASCHINENCODE` (`ºº: <IO> :ºº`) — beide existieren als MCS-Datei, aber ihr
Inhalt ist ein Komponentenplatzhalter.

**Für einen Referenzlauf braucht es das nicht.** Der Vertrag ist auf
MCS-Ebene vollständig: die ALU bekommt drei Werte (R1, Opcode, R2) in dieser
Reihenfolge. Eine Referenz-ALU kann man danach schreiben und `1 + 1` rechnen,
ohne eine Zeile Assembly — nur um zu zeigen, dass die Kette trägt.
