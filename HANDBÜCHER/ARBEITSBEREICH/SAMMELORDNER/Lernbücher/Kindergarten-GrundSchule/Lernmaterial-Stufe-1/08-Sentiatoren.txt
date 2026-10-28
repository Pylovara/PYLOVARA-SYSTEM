# Kapitel 8: Der magische Wächter (Sentiatoren)

Hallo kleiner Entdecker! 🌟🚪

Du hast schon gelernt, wie man Dinge in Kisten packt und
wie der Aktionsdraht sie wach macht. Aber was passiert, 
wenn wir mal nicht wissen, ob etwas in der Kiste ist? Oder 
wenn wir uns entscheiden müssen?

Dafür gibt es den Magischen IgelWächter mit seinem 
**Entscheidungszauber** (Sentiator), der Zwei Verschiedene 
Wege Zaubern kann.

(Bild: ![Hier kommt das Bild für den Entscheidungszauber hin](../../SonderSchule/Bildliches-Stufe-1/Sentiatoren.png))

---

## Die Symbole: ¶ und ¶¶

Es gibt zwei Wege, die der IgelWächter uns zeigen kann:

1. **¶ (Der Ja-Weg):** 
   Dieses Zeichen bedeutet: "Ja, alles ist okay! 
   Der Apfel ist da!" oder "Die Kiste ist bereit!" 
   Wenn der IgelWächter dieses Zeichen zeigt, 
   dürfen wir weitergehen.
   (Bild: Hier kommt das Bild für den Entscheidungszauber 
   JA hin)


2. **¶¶ (Der Nein-Weg):** 
   Dieses Zeichen bedeutet: "Stopp! Da fehlt etwas!" 
   oder "Der Apfel ist weg!"
   Dann zeigt uns der IgelWächter einen anderen Weg.
   (Bild: Hier kommt das Bild für den Entscheidungszauber
    NEIN hin)

Mit dem **Entscheidungszauber** können wir entscheiden, 
was passieren soll, je nachdem, ob was in der Kiste ist
oder nicht.

---

## Schauen wir mal in die Apfel-Kiste:

Stell dir vor, wir haben eine Kiste: `["Apfel"]`. 
Wir wollen wissen, ob der Apfel noch da ist, damit wir 
ihn essen können.

¢!

["Apfel"]  # Wir legen den Apfel bereit

¶ ¬· »["Apfel essen"]« 
# Der Wächter prüft mit einem Zaubert und einem 
# Meister Rex Tisch ob der Apfel da ist? 
# Wenn JA (¶), dann dürfen wir ihn essen und 
# Meister Rex kann uns ihn geben!

¶¶ ¬· »["Suchen gehen"]« 
# Wenn NEIN (¶¶) Zaubert Der Wächter mit Meister Rex einen 
# Tisch und wir müssen nach dem Apfel suchen, weil der 
# Apfel weg ist, 
# Wenn Nein dann Zaubert der Wächter Mit Meister Rex den 
# Such Tisch!

!¢

---

## Die Maus und der Käse 🧀

Die kleine Maus möchte wissen, ob Käse im Haus ist.

¢!

["Käse"]

¶ ¬· »["Käse mampfen"]«
¶¶ ¬· »["Traurig piepsen"]«

!¢

---

## Das Eichelhörnchen und seine Nuss️ 🌰 🐿️

¢!

["Nuss"]

¶ ¬· »["Nuss knacken"]«
      ¬· »["Nuss essen"]«

¶¶ ¬· »["Traurig piepsen"]«
       ¬· »["Suchen gehen"]«
           ¬· »["Bäumen klettern"]«
           ¬· »["Boden absuchen"]«
           ¬· »["Andere Tiere fragen"]«
               ¶ ¬· »["Nuss gefunden"]«
                     ¬· »["Nuss knacken"]«
               
               ¶¶ ¬· »["Traurig piepsen"]«
                      ¬· »["Geht nach Hause"]«
                          ¬· ["Schläft ein"]
!¢

---
### Deine Aufgaben:

1. **Wächter-Check:** Welches Zeichen benutzt der Wächter,
wenn alles super ist und wir weitermachen dürfen? 
Einmal `¶` oder zweimal `¶¶`?

2. **Malen:** Zeichne ein großes Tor. Auf der einen Seite
malst du ein `¶` (für "Alles da!") und einen glücklichen 
Smiley. Auf der anderen Seite malst du ein `¶¶` (für "Oh 
nein, leer!") und einen fragenden Smiley.

3. **Zauber schreiben:** Kannst du den Entscheidungszauber 
JA und NEIN für eine Katze schreiben, die `["Milch"]` 
trinken will?
   *(Tipp: `¶ ¬· »["Milch trinken"]«` Vergiss den 
   Aktionszauber nicht!)*

---

> [!TIP]
> **Für Eltern:** Ein **Sentiator** (Entscheidungszauber) 
> (`¶` Ja oder `¶¶` Nein) ist in Pylovara eine 
> **bedingte Anweisung** (Logik-Gatter / If-Else). 
> - `¶` (SENTAN-WENN-KANN) prüft, ob die Bedingung (der 
> Wahrheitswert oder der physikalische Status) erfüllt ist. 
> - `¶¶` (SENTAN-WENN-NICHT) reagiert auf den negativen 
> Zustand oder einen Fehler.
> - Helfen Sie dem Kind zu verstehen, dass das System hier 
> "denkt" und eine Entscheidung trifft, basierend auf dem, 
> was in der Kiste gefunden wird.

> [!IMPORTANT]
> **Für Eltern:** Technisch gesehen prüft der Sentiator den 
> Zustand des **lcore-nodes** oder den **Truth-Status** 
> eines Proteins. In dieser Lernstufe führen wir das 
> Konzept der Entscheidung ein: Aktionen werden nur dann 
> ausgeführt, wenn die logische Voraussetzung gegeben ist. 
> Das schult das Verständnis für Kausalität ("Wenn... 
> dann...").
