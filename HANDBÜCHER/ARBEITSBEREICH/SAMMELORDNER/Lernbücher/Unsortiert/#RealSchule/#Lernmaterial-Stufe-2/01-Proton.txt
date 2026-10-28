# Kapitel 1: Die kleine Zauberkiste (Proton)

Hallo kleiner Entwickler! 🌟

Du bist ja schon ein richtiger Profi! Du weißt bereits, wie wir mit dem **Zauberrand** (Transaktionsrahmen) `¢! ... !¢` einen sicheren Bereich schaffen und wie **Meister REX** (Workspace) dort die Arbeitstische aufstellt. Du kennst auch schon die großen **Zauberkisten** (Protein) `[]`, in denen wir unsere Entdeckungen speichern.

Aber heute wird es noch magischer! Stell dir vor, du hast einen Affen 🐒 in deiner Zauberkiste. Der Affe hat aber Hunger und möchte seine eigene kleine **Lunchbox** mitnehmen, in der seine Bananen sind. 

Wie machen wir das? Dafür gibt es einen neuen Zauber: das **Proton**. Aber wir nennen es einfach: **Die kleine Zauberkiste**.

---

## Das Symbol für die kleine Kiste: {}

Die kleine Zauberkiste (das Proton) sieht fast so aus wie die große, aber sie ist etwas geschwungener, wie zwei kleine Arme, die etwas festhalten:

# {}

## Ein Tier mit seinem Proviant

Wenn wir einem Tier etwas mitgeben wollen, legen wir die kleine Kiste (Proton) einfach mit in die große Kiste (Protein). Wir trennen sie mit unserer **Zaubertrennwand** (Trennbefehl) `|`.

Schau mal:

### »["Affe" | {"Bananen"}]«

Das bedeutet: Der Affe ist in der großen Kiste und er hat seine kleine Lunchbox mit den Bananen dabei! 🍌

Oder wenn er auch Spielezeug mitnehmen möchte:

### »["Hund" | {"Hundefutter"} | {"Ball"}]«

Hier hat der Hund gleich zwei kleine Kisten dabei: eine für sein Essen und eine für sein Spielzeug!

---

## Schlafende und Wache Kisten

Weißt du noch? Mit dem **Aktionszauber** (Aktionsdraht) `» ... «` machen wir Dinge wach. 

- `["Affe" | {"Bananen"}]` – Hier schläft alles. Der Affe und seine Bananen warten nur. (**Schlafende Kiste**)
- `»["Affe" | {"Bananen"}]«` – Zack! Der Aktionszauber weckt sie auf. Jetzt wird der Affe gefüttert! (**Wache Kiste**)

---

### Deine Aufgaben:

1. **Malen:** Zeichne einen großen Hund 🐕 in eine Zauberkiste `[]`.
2. **Lunchbox:** Zeichne eine kleine Zauberkiste `{}` daneben (in die große Kiste) und mal ein paar leckere Knochen hinein!
3. **Zauber schreiben:** Kannst du eine der zwei kisten für deinen Hund mit seinen Knochen aufschreiben? 
   *(Tipp: `["Hund" | {"Knochen"}]`)*

---

> [!TIP]
> **Für Eltern:** Ein **Proton** `{}` ist in Pylovara eine Unter-Struktur innerhalb eines Proteins. Es dient primär dazu, **Zustände (States)** oder **Hardware-Parameter** zu definieren.
> - Während das **Protein** `[]` den allgemeinen Daten-Container darstellt, steuert das **Proton** `{}` die spezifischen Details oder "Versorgungs-Werte" (wie z.B. Spannung, Farbe, oder hier: Futter).
> - Protons stehen in der Syntax oft am Ende oder als ergänzende Information zu einem Objekt.
> - Helfen Sie dem Kind zu verstehen, dass die geschweiften Klammern `{}` etwas "Zugehöriges" markieren – wie ein Rucksack, den man trägt.

> [!IMPORTANT]
> **Für Eltern:** Technisch gesehen werden Protons genutzt, um Hardware-Komponenten (Gatter/Register) direkt mit Werten zu versorgen. In der Lernstufe 2 führen wir dies als "kleine Kiste" ein, um die hierarchische Struktur von Daten (verschachtelte Objekte/States) begreifbar zu machen.
