# Nutzungs-Syntax — Kernel 805

Quelle: `kernel-nr: 805`, `MASCHINEN-CODE-SPEECH-SYNTAX-LAW`, Stand
`[01:57:47 | Sa 25 Apr 2026]`. Älter als 00-83, ausdrücklich als
„NUTZUNGSBROSCHÜRE" bezeichnet, nicht als Lernmaterial.

Hier stehen zum ersten Mal **konkrete Schreibweisen** für jede Komponente.

## Der Zyklus

```
3ZYKLUS-OUT:        ← Ausgang, von innen nach außen gelesen
      1SYNC:
      2KOPF:
     1START:
      2FOOD:
 3ZYKLUS-IN:        ← Eingang
       4SEN:
       5REX:  (mehrfach, parallel)
      6FREE:
```

> „8 KERN PRINZIP | WIRD NOCH ERWEITERT"

## Protein

```mcs
¢!
  »—[ ]«    # AUSFÜHRBARES PIPE PROTEIN
!¢

¢!
   [ ]     # UNAUSFÜHRBARES STATUS- ODER TRANSPORT-PROTEIN
!¢
```

Und der Strang:

```
  *              # ZUFÜHRDATEN | KOPF
  ↑
 »—[ ]«          # AUSFÜHRBARES PIPE PROTEIN
  ↓
  *              # VERTIKALER PROZESS-STRANG | PIPE
```

Die Pfeile sind Teil der Notation — **die Richtung steht im Code**, nicht in
einer Nebendatei.

## Die drei Proteine einer Verarbeitung

```
  ¿                                    # SYNAPTOR-KNOTENPUNKT
  ↑
  ÷ »[ HEADER PROTEIN ]«               # KOPF: lässt Daten einfließen
  ↑
 »—[ MAIN PROTEIN ]«                   # PIPE: Bearbeitungspunkt
  ↓
  ± »[ FOOD PROTEIN ]«                 # FOOD: Speichermedium, FF führend
  ↓
```

## NITROX — der Cache-Dreiklang

Das ist die wichtigste neue Form. Ein Nitrox-Statement hat **drei Felder**,
getrennt durch `¥`:

```
 ± [(<NR>)—Ø ¥ <CACHE> ¥ Ø]     # FOOD: greift ab, speichert im Cache   (FF)
 ÷ [Ø ¥ <CACHE> ¥ Ø—(<NR>)]     # KOPF: gibt aus dem Cache in den Takt   (FT)
```

Feld 1 = Adresse · Feld 2 = Inhalt · Feld 3 = Gegenstelle.

Und die ALU-Rechnung im Zusammenhang:

```mcs
 »—[× 1 + 1 ×]«
  ± [(1)—Ø ¥ 1 + 1 ¥ Ø]              # Ergebnis 2 in Cache (1) ablegen

  ¬·¤= »—[" 2 "]«                     # REX, Fläche 1, Koordinator, ALU-Rückwert

        ± [(2)—Ø ¥ 2 ¥ Ø]             # 2 in Cache (2)
        ↓   ÷ [Ø ¥ 2 ¥ Ø—(2)]         # (2) in den Takt
        ↓   ÷ [Ø ¥ 1 + 1 ¥ Ø—(1)]     # (1) in den Takt
        ¬· »—[× 4 + 4 + (1) + (2) ×]« # neue Rechnung mit beiden
```

**Damit ist die ALU-Kante vollständig:** die Boxi trägt den Ausdruck, die
Nitrox-Statements holen und schieben die Werte.

## Die übrigen Komponenten

| Komponente | Schreibweise | Bedeutung |
|---|---|---|
| Vatron | `© »—[ ]«` | Firewall-Kontrolle vor dem Protein |
| Navigator | `$ »[ pfad ]«` | Userland-Zugriff |
| Navigator | `§ »[ pfad ]«` | interner Kernel-Zugriff |
| Boxis | `»[<BOXK>]«` | Boxi im Protein |
| Proton | `»[<POOK>]«` | Hardware-Versorgung |
| Boxi+Proton | `»[<BOXK>\|<POOK>]«` | kombiniert |
| Argument | `»[<BOXK>]«««V<NR>` | Variable anlegen (Baumodus) |
| Argument | `»[<BOXK>]«««<TIMER><ZEIT>` | Zeittaktung (NS, S, T) |
| Argument | `»[<BOXK>]«««<COMMAND>` | Befehl (B, E, ſ) |
| Synaptor | `¿¹` / `¬·¤›` | Prozess-Verbindungsknoten |
| Zytron | `£1` … `£1` | Schleife: Ausgang oben, Eingang unten |
| Sentiator | `¶ ¬·` / `¶¶ ¬·` | WENNKANN / WENNNICHT + REX-Fläche |
| REX | `¬·`, `¬··`, `¬·¤` | Fläche 1, Fläche 2, mit Koordination |

## Was das gegenüber 00-83 ändert

1. **Die Richtung steht im Code**, als Pfeil `↓` / `↑` zwischen den Zeilen —
   nicht nur als Spaltenpräfix wie in 00-83 (Modus 0). 805 ist damit die
   ältere, unmittelbarere Form.
2. **`¥` ist der Cache-Trenner.** Dieselbe Rolle spielt `¨` in
   `mcs_alufix1.mcs` (`×¨(1)¨ > ¨(2)¨×`). Beide sind Marker für dieselbe
   Sache: „hier steht ein Wert, der aus dem Cache kommt".
3. **Die Nitrox-Statements machen ÷ und ± erst verständlich.** `÷` schiebt in
   den Takt, `±` holt aus dem Takt — und beide nennen ihren Cache explizit.

## Was noch fehlt

Die Beispiele sind Nutzung, kein Gesetz: es gibt **keine Grammatik**, die
sagt, welche Zeile auf welche folgen darf. 805 zeigt, was möglich ist, nicht
was erlaubt ist. Für einen Prüfer bräuchte es beides.

## Der Vergleich mit der 7.x-Notation

Dieselbe Sache, zwei Schreibweisen:

```
 805 (April 2026)                          7.x (August 2026)

 ÷ [Ø ¥ 2 ¥ Ø—(2)]                        ÷ Ø—(1): ** :Ø
   KOPF: gibt 2 aus Cache (2) in den Takt    Cache (1) liefert **
   ± [(2)—Ø ¥ 2 ¥ Ø]                        ± Ø: ** :(1)—Ø
   FOOD: legt 2 in Cache (2) ab               ** nimmt aus Cache (1)
```

Das sieht zuerst nach einem Tausch aus — in 805 schreibt `±` in den Cache, in
7.x schreibt `÷` auf die Adresse. Es ist aber keiner, wenn man die Adresse
als **Cache-Platz** liest und den Wert als **Prozess-Größe**:

- `÷` beschreibt die Verbindung **vom Cache in den Prozess**.
- `±` beschreibt dieselbe Verbindung **vom Prozess in den Cache**.

Beide Sätze beschreiben denselben Draht von seinen zwei Enden. **Deshalb
treten sie in der Bibliothek immer paarweise auf** — 20 Zuweisungen, 20
Abrufe im Zertifikat, keine Waise. 805 macht den Cache-Inhalt explizit
(`¥ 2 ¥`), 7.x nennt nur den Symbolnamen (`**`).

Das ist die Auflösung, die zu beiden Notationen passt. Wenn sie falsch ist,
ist sie falsch an genau einer Stelle: der Leserichtung von `÷` und `±`.
