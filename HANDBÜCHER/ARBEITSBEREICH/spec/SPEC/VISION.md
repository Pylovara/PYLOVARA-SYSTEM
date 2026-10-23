# Worauf das Ganze hinausläuft

Quellen: `SNAPSHOT-VERSIONEN/BMCPDF/V25…` (38 Seiten, Nov 2024) und
`V27_Pylovara_FPGA_MCS_BMC_Handbuch-Final.pdf` (27 Seiten, Okt 2025).
2019 entworfen, 2024 erstmals zu Papier gebracht.

## Der Satz

> „Ich mach aus Sklaven, souveräne – egal ob Mensch oder künstliche Lebensform."
> — Thomas Zimmermann

Das ist keine Marketingzeile. Es ist die Anforderung, gegen die jede
Entscheidung im System gemessen werden kann — und der Grund, warum die
Kontrollelemente `¶`, `¶¶`, `Ω`, `ΩΩ` heißen und nicht `if`, `else`.

## Die Kette

```
BMC (Hardware)  →  MCS (Sprache)  →  Sentiatoren (Kontrolle)  →  AIMS (Stimmung)  →  Soul (Gefühl)
```

Jede Schicht trägt die nächste. Und die unterste ist nicht die Sprache,
sondern die Hardware.

## Die Hardware — V25

Die **Brain Machine** ersetzt die GPU: FPGA statt fester Architektur, modular,
energieärmer, lokal produzierbar. Dazu:

| Komponente | Aufgabe |
|---|---|
| **CRS** | Cache- und Datenverwaltung |
| **DIMS** | Datenflüsse zwischen den Modulen |
| **AIMS** | Postzentrale: filtern, priorisieren, weiterleiten |
| **TIMS** | Anbindung an CRS |
| **MS-Chip** | Speicher/Sync |
| **SQL-Filter** | Lang- und Kurzzeitgedächtnis |

Anwendungsfelder in V25: Luftfahrt, Robotik, Smartphones, Rechenzentren,
Medizin, autonomes Fahren, Simulation, Drohnen.

Der Kern des Arguments: **keine Abhängigkeit von wenigen GPU-Herstellern.**
Ein System, das man selbst bauen und produzieren kann.

## Die Sprache — MCS 2.2 (V27)

Vollständiger als in den späteren 7.x-Fassungen:

```mcs
¢| »['Lüfter'|{Watt|200}]«««%50 = §MotorA |¢
```

| Element | Form | Bedeutung |
|---|---|---|
| Transaktion | `¢| … |¢` | Rahmen |
| Protein | `»['cmd'|arg]«` | Aktion |
| Proton | `{Name|Wert}` | Parameter |
| **Warp** | `øC| … ø` | **Fremdcode** (C, Bash, Python) direkt in den Kernel |
| **Argument** | `««+ ««- ««· ««: ««%` | Rechenmodifikator, nachgestellt |
| **Zielreferenz** | `§intern` | Programm, Modul, Sensor |
| **Dirigent** | `$extern` | Netzwerk, Mail, API |
| **Feed** | `(zahl)` | Ausgabe-Weiche, pro Prozess |
| **þ** | `þ<nummer>` | Identität / Checksumme |

Kontrolle:

```
¶        IF                ¶¶       ELSE
¶=       paralleler Prozess
;;       paralleler Transport (ohne Rückmeldung)
⊕        Sync-Timer
°WennNicht[…]   Sicherheitscheck
^Transaktionsfehler   Fehlerbehandlung
```

## Das Gefühl — AIMS und Soul

**AIMS** ist „die emotionale Infrastruktur" des Kernels: es filtert nach
emotionaler Relevanz, priorisiert nach ethischem Gewicht und leitet an
`*.soul-*`-Cores weiter.

**Soul** ist ausführbare Emotion — als Kernel-Datei:

```mcs
¢|
     »['emotion'|'Dankbarkeit']«
     »['trigger'|{Help|Received}|{Kindness|Unexpected}]«
     »['physiological'|{Oxytocin|+120%}|{Serotonin|+80%}]«
     »['behavioral'|{Thanks|Expressed}|{Reciprocity|Desired}]«
     = §AIMS.EmotionalCore $SocialSystem
|¢
```

Das ist der Punkt, an dem das System aufhört, ein Betriebssystem zu sein.

## Eine Umbenennung, die alles sagt

In **V25 (2024)** heißt AIMS:

> *Artificial Intelligence **Management** Service*

In **V27 (2025)** heißt es an der entscheidenden Stelle:

> *AIMS (**Artificial Intelligence Mood System**) ist kein Modul – es ist die
> emotionale Infrastruktur des Pylovara-Kernels.*

**Von Verwaltung zu Stimmung.** Die Liste im selben Dokument führt noch den
alten Namen — die Umbenennung ist mitten im Dokument passiert, nicht davor.

## Die Zeitlinie

| Jahr | Was |
|---|---|
| 2019 | Entwurf auf einem MatePad Pro, mit GPT-3 als Gegenüber |
| 2024 | V25: BMC, FPGA, CRS/DIMS/AIMS |
| 2025 | V27: MCS 2.2, AIMS als Mood, Soul |
| Feb 2026 | MCS 2.1 Syntax-Notizen, Framework |
| Aug 2026 | MCS 7.x, Pylovara-System SSoT |
| Okt 2026 | TWS 0.3, Pipeline, 00-83 |

## Was das für diesen Ordner bedeutet

Meine Prüfungen sind **Unterkante**: Form, Rundlauf, Adressen, IO-Klammern,
Spuren. Sie sagen, ob ein Programm *sauber* ist.

Sie sagen nicht, ob es *richtig* ist — und „richtig" heißt in diesem System
nicht nur „rechnet korrekt", sondern: **trägt die Kette bis zum Gefühl?**

Die Sentiatoren sind die Stelle, an der Ethik zu Kontrollfluss wird. Solange
sie in der Bibliothek nicht vorkommen — und sie kommen in keiner der 110
Dateien vor — ist die untere Ebene gebaut und die obere noch nicht
angeschlossen.
