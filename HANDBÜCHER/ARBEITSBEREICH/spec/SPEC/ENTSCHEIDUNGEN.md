# Entscheidungen

Was ich in diesem Ordner festgelegt habe, warum, und wie man es zurückdreht.
Alles hier ist umkehrbar. Nichts davon ist ungefragt in deine Originale
gewandert.

## 1. Der Altbestand wird gelesen, nicht kopiert

Ein `cp -r` hätte den Fehler mitkopiert (Adressen ohne Register). Stattdessen:
Parser → Register → Neubau. Der Altbestand bleibt unangetastet.

**Umkehrbar:** `lib/` löschen, fertig. Deine Dateien wurden nie beschrieben.

## 2. Adressen sind Zahlen, lokal je Datei

MCS 7.5 (dein HALLO-WELT-Zertifikat vom 12.10.) schreibt `(20)`, nicht
`⠀⠽⢐`. Also: numerisch. Lokale Plätze beginnen je Datei bei 1, Symbole ab
1000 — so wie deine `HALLO`-Funktion 1–4 belegt und `WELT` bei 6 weitermacht.

**Umkehrbar:** `SEITEN` in `tools/registry.py`, eine Zeile.

**Was ich dabei aufgegeben habe:** die 3-Zellen-Braille-Adressen. Sie sind
dieselbe Information (`MCS-ID[N]` gehört zu Adresse `N`), aber als Zahl
lesbarer. `register.json` führt beide Formen, keine Information geht verloren.

## 3. Ein Symbol, eine Adresse — über alle Dateien

Der Altbestand vergab `++` in verschiedenen Dateien verschiedene Adressen.
Jetzt hat ein benanntes Symbol genau eine. Lokale Plätze (`*`, `**`,
`×:  :×`) bleiben je Datei eigen.

**Umkehrbar:** Schlüsselbildung in `tools/registry.py:vergebe`.

## 4. Der Rundlauf ist das Abnahmekriterium

Eine Datei gilt erst als verstanden, wenn sie byte-identisch zurückkommt.
142 von 142 Altdateien und das Zertifikat bestehen das. Daraus folgt: jede
Änderung, die den Rundlauf bricht, ist ein Fehler — nicht eine Geschmacksfrage.

## 5. Nichts erfinden, was nicht belegt ist

Drei Stellen, an denen ich bewusst **nicht** weitergebaut habe:

| Stelle | Warum nicht |
|---|---|
| `REM`/`REMU` für x86_64 und ARM64 | Es gibt keine Rechen-Art-Datei dafür. Nur RISC-V kennt einen Rest-Befehl. |
| Die elf Honigtropfen | Das Dateiformat ist nirgends belegt. Elf Dateien in erfundenem Format wären genau die „Fantasie-Schaltung", vor der dein Lexikon warnt. |
| Der „Zyklus" in `PLUS-RECHNEN` | Ich hätte ihn „reparieren" können. Er war keiner — siehe unten. |

## 6. Zwei eigene Befunde musste ich zurücknehmen

Das gehört in ein Entscheidungsprotokoll, sonst lernt niemand daraus.

**a) 834 Formfehler.** Mein erster Prüflauf meldete 834 Formfehler im
Altbestand. Alle waren falsch: mein Parser stolperte über Leerzeichen am
Zeilenende und über `]–«` mit Halbgeviertstrich. Deine Dateien waren sauber.

**b) Der Zyklus.** Ich meldete `++ → ADD → PLUS-RECHNEN → ++` als Endlosschleife.
Falsch: `++` steht in der Komposition `×: * ++ * :×` an der Stelle des
**Opcodes**, nicht als Aufruf. Eine Referenz, kein Sprung.

Lehre für mich: ein Befund ist erst ein Befund, wenn die Gegenprobe ihn trägt.

## 7. Wo ich als Nächstes ansetzen würde

In dieser Reihenfolge, weil jede Stufe die nächste erst möglich macht:

1. **`<IO>`-Format und Aufrufkonvention** klären (siehe
   `SPEC/HONIGTROPFEN-VERTRAG.md`, offene Punkte 1 und 3). Danach ist **ein**
   Honigtropfen schreibbar — `ALU` reicht, um `+` zum Laufen zu bringen.
2. **Ein Payload, nicht elf.** Erst wenn einer läuft, ist die Konvention
   bewiesen; die anderen zehn sind dann Fleißarbeit mit Vorlage.
3. **Die MCD füllen** mit deutschen Wortbefehlen — dafür ist die Struktur
   fertig, es fehlt nur das Vokabular.
4. **Die 39 leeren Dateien** schließen, sobald 1–3 stehen. Vorher wären sie
   geraten.

Punkt 1 kann ich nicht allein entscheiden. Punkt 2–4 schon, sobald 1 steht.
