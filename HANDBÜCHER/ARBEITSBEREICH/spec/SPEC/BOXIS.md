# Die elf BOXIS und ihre Honigtropfen

Quelle: `09-BOXIS.KERNEL-NOTES` · Bibliothek: `BOXIS/`

| # | Komponente | Programm-ID | Honigtropfen | Honigtropfen-Muster | Datei |
|---:|---|---|---|---|---|
| 1 | **PRINT** | `⠆⢂⡈⠊` | `⠆⢂⡄⡠` | `06 82 44 60` | `BX-PRINT` |
| 2 | **ALU** | `⠆⢂⡄⢠` | `⠆⢂⡄⠰` | `06 82 44 30` | `BX-ALU` |
| 3 | **SYSCALL** | `⠆⢂⡂⢁` | `⠆⢂⡂⠔` | `06 82 42 14` | `BX-SYSCALL` |
| 4 | **ABI** | `⠆⢂⡂⠰` | `⠆⢂⡂⠅` | `06 82 42 05` | `BX-ABI` |
| 5 | **SHELL** | `⠆⢂⡁⠔` | `⠆⢂⠰⢄` | `06 82 30 84` | `BX-SHELL` |
| 6 | **EINGABE** | `⠆⢂⡁⠃` | `⠆⢂⠰⡂` | `06 82 30 42` | `BX.EINGABE` |
| 7 | **MCD** | `⠆⢂⠨⢐` | `⠆⢂⠨⠤` | `06 82 28 24` | `BX-MCD` |
| 8 | **PFAD** | `⠆⢂⠨⡈` | `⠆⢂⠤⠔` | `06 82 24 14` | `BX-PFAD` |
| 9 | **NET** | `⠆⢂⠤⠰` | `⠆⢂⠤⠊` | `06 82 24 0A` | `BX-NET` |
| 10 | **OPCODE** | `⠆⢂⠢⠡` | `⠆⢂⠡⣀` | `06 82 21 C0` | `BX-OPCODE` |
| 11 | **MASCHINENCODE** | `⠆⢂⠢⠌` | `⠆⢂⠡⢁` | `06 82 21 81` | `BX-MASCHINENCODE` |

## Was der Honigtropfen ist

Jede Komponente hat eine `KINDERWABEN-KATEGORIE SYSTEMWABE` mit einem
`HONIGTROPFEN <ART>`. Der Honigtropfen ist der Platz, an dem die Assembly
steht — bis MCS sich selbst trägt. Die Kernel-Seite gleicht die Eingabe
dagegen ab (`⟗` = NAME ABGLEICHEN) und gibt bei Treffer aus.

```
 KINDERWABEN-KATEGORIE <ID> SYSTEMWABE
 HONIGTROPFEN <ID> <ART>
  # AXIOM | (EINGABE ABGLEICHEN)
  AXIOM ¡ <EINGABEBEHALTEN> ⟗ ⧖⧖ <HONIGTROPFEN> Ξ <AUSGABE>
```
