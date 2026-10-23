# Kette — und die Korrektur meines eigenen Befunds

Stand: 23.10.2026 · geprüft gegen `lib/` (110 Dateien)

## Der Aufbau

```
Tastatur (70)            Taste → Adresse, nativ: ReadKeyStroke()
      │
Rechen-Symbole (12)      ++  --  ××  ÷÷  X÷+  X÷-  A÷+  A÷-  R÷+  R÷-  RR÷+  RR÷-
      │  $: Pfad
Operatoren (10)          ADD SUB MUL DIV IDIV SDIV UDIV DIVU REM REMU
      │  $: Pfad je ISA
Rechen-Arten (18)        PLUS-RECHNEN, MINUS-RECHNEN, MAL-RECHNEN, GETEILT-…
      │  ALU-Aufruf
BOXIS (11)               BX-ALU
      │
Honigtropfen             ⠆⢂⡄⠰  ← hier steht die Assembly
      │
      ✗                  Assembly fehlt noch
```

## Korrektur: der „Zyklus" war keiner

Ich hatte gemeldet, die Kette drehe sich im Kreis:

```
++  ──$:──▶  ADD  ──$:──▶  PLUS-RECHNEN  ──ruft──▶  ++
```

Das war eine Fehllesung der Notation. `PLUS-RECHNEN` **ruft `++` nicht auf**,
es liest `++` als **Opcode** und gibt ihn an die ALU weiter. Der Beweis steht
in der Ausgabekomposition derselben Datei:

```
 ¬· »—[(3 (1) (1013) (2) )]—«
     ¬·¤ »[×: * ++ * :×]«««~: BAUMODUS FUNKTION SPEICHER AUSGANG :~«
```

Das liest sich als `ALU(R1, ++, R2)` — `++` steht **zwischen** den Operanden,
an der Stelle des Opcodes. Eine Referenz, kein Sprung.

Damit ist die Kette nicht zyklisch, sondern vollständig:

```
++  ──$:──▶  ADD  ──$:──▶  PLUS-RECHNEN  ──×:──▶  BX-ALU  ──▶  Honigtropfen ⠆⢂⡄⠰
```

Was fehlt, ist nicht der Abschluss, sondern **der Inhalt des Honigtropfens**:
die Assembly. Genau die Stelle, die Thomas als „vorerst Assembly, bis es mit
der eigenen Sprache geht" beschrieben hat.

## Die elf Honigtropfen

Siehe `SPEC/BOXIS.md` — erzeugt von `tools/boxis.py` aus der Kernel-Note
`09-BOXIS.KERNEL-NOTES`, nicht abgetippt.

Jeder Honigtropfen sitzt in einer `KINDERWABEN-KATEGORIE SYSTEMWABE` und wird
vom Kernel über `⟗` (NAME ABGLEICHEN) gegen die Eingabe geprüft:

```
 HONIGTROPFEN ⠆⢂⡄⠰ <ART>
  AXIOM ¡ <EINGABEBEHALTEN> ⟗ ⧖⧖ <HONIGTROPFEN> Ξ <AUSGABE>
```
