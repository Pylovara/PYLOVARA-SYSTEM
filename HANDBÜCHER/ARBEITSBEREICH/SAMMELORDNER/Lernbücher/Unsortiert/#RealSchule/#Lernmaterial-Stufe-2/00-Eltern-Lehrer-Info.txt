# Information für Eltern und Lehrkräfte: Willkommen in Stufe 2!

Liebe Eltern, liebe Lehrkräfte,

herzlichen Glückwunsch! Die Kinder haben die ersten Grundlagen unserer Lernwelt (Stufe 0 und 1) erfolgreich gemeistert. 

In dieser Stufe führen wir das Konzept der **Datenstrukturierung** und **Zustandsverwaltung** ein. Wir erweitern die abstrakten Konzepte von Stufe 1 (Transaktionsrahmen, Protein, Arbeitsfläche, REX) um die praktische Anwendung in der digitalen Welt verbessert zu vermitteln.

## Die neuen Konzepte im Überblick:

### 1. Das Proton `{}` (Kleine Zauberkiste)
- **Abstraktion:** Ein Container für spezifische Werte oder Zustände.
- **Analogie:** Die Lunchbox des Affen. Während die große Kiste `[]` das Objekt selbst ist, enthält die kleine Kiste `{}` dessen Eigenschaften (Attribute).
- **Technische Relevanz:** Entspricht der Definition von **Objektattributen** oder **Konfigurationsparametern** in der Programmierung.

### 2. Schlafende vs. Wache Kisten
- **Schlafend:** Daten liegen bereit, aber es passiert nichts. (Statische Daten)
- **Wach:** Daten werden aktiv verarbeitet. (Dynamische Daten / Execution)
- **Technisch:** Dies simuliert den Unterschied zwischen **Datenspeicherung** (RAM/Festplatte) und **Datenverarbeitung** (CPU/Execution Thread).

## Didaktischer Ansatz
Wir nutzen weiterhin die **bildliche Metapher**, um komplexe technische Sachverhalte greifbar zu machen. Das Kind lernt nicht „Syntax“, sondern „Logik“. Indem es dem Hund sein Futter in eine kleine Kiste packt, versteht es intuitiv das Prinzip von **Objekt + Zustand**. Vermitteln Sie dabei spielerisch, dass die Namen der Kisten zweitrangig sind – entscheidend für die Funktion sind die Symbole `{}` für die kleine und `[]` für die große Kiste.

## Wichtige Hinweise für die Begleitung
- **Geduld:** Die Unterscheidung zwischen `[]` (Container) und `{}` (Inhalt/Zustand) kann anfangs ungewohnt sein. Üben Sie gemeinsam das „Packen“ und „Auspacken“.
- **Kreativität fördern:** Ermutigen Sie das Kind, eigene „Lunchboxen“ für seine Spielfiguren zu erfinden.

## Wenn Sie möchten:
- **Tastatur:** Helfen Sie beim Finden der Klammertasten auf der Tastatur. Dies ist die erste praktische Berührung mit „echter“ Programmier-Syntax.

Damit legen wir den Grundstein für echtes logisches Denken und bereiten die Kinder spielerisch auf die Welt der Programmierung vor. Es geht dabei weniger um das bloße Auswendiglernen von Zeichenfolgen, sondern vielmehr um das tiefe Verständnis von Strukturen und Abläufen. Die Symbole dienen als Brücke, um diese abstrakten Konzepte begreifbar zu machen – eine Wissensbasis, die den Weg für spätere native Softwareentwicklung (bis hin zu klassischen Umgebungen wie C, GCC oder GNU) ebnet.

wir arbeiten von unseren Standpunkt ausgesehen rückwärts, das heißt wir bringen den Kindern die Konzepte bei und nicht die Syntax. Die Syntax kommt später ganz von alleine. 

Beispiel für eine erweiterte 

`KAFFEESTARTER.KAFFEECREMAMASCHINE-CORE` # In der Datei Aktualisieren wir dann:

¢!

Ŧ¹ »["KNOPF AN"|{Volt:230, Watt:1000, Kapazität:50.0L}]« # 50 liter wirds nicht geben ;-) 
    ¬·»["Kaffeemaschine"|{Volt:230, Watt:800, Kapazität:1.0L}]«««T28 
    # beispiel : 28 Takte Wartezeit wegen dem Heizstab
       ¶ ¬· »["Tanken"|{Grenze:1.0L}]«««T9
             ¬· »["Brauch Wasser"|{Liter:1.0L}]«
                 ¶ ¬· »["Wasser voll"|{Zufuhr:Aus}]«
                       ¬· »["Stoppe die Zufuhr"]«««E
                 ¶¶ ¬· »["Wasser rein"]«
                        ¬· »["Füllstand prüfen"|{Liter:1.0L}]«««S8
                            ¶ ¬· »["Füllstand voll"]«««E
                            ¶¶ ¬· »["Füllstand leer"]«
                                   ¬· »["FEHLER-Meldung"]«

             ¬· »["Brauch Strom"]«««T4
                 ¶ ¬· »["Heizstab an"|{Watt:800}]«««E
                 ¶¶ ¬· »["Heizstab aus"|{Watt:0}]«
                        ¬· »["Fehler-Meldung"]«
                        
             ¬· »["Brauch Kaffeepulver"]«««T4                
                 ¶ ¬· »["Kaffeepulver ist voll"|{Pulver:0g}]«««E
                 ¶¶ ¬· »["Kaffeepulver ist leer"|{Pulver:100g}]«
                        ¬· »["FEHLER-Meldung"]«

             ¬· »["Brauch Tasse"]«««T7
                 ¶ ¬· »["Tasse ist da"]«««E
                 ¶¶ ¬· »["Tasse ist nicht da"]«
                        ¬· »["Wir stellen schnell eine darunter"|{Tasse:1}]«
                            ¬· »["Brauch Milch"|{L:100ml}]«««S1
                            ¬· »["Brauch Zucker"|{Zucker:10g}]«««S2
                            ¬· »["Brauch Löffel"|{Löffel:1}]«««S3

    ¶¶ ¬· »["ES FEHLTE ETWAS"]«««T4
        ¬· »["Kaffee ist nicht fertig"]«
            ¬· »["Maschinen-Signal"|{Signal:1}]«
                ſ »["Kaffee ist nicht fertig, reinige aber den Speicher!"]«

!¢

Sie verstehen also das ihr in diesem Punkt die Syntax der MCS-Sprache lernt. Aber nicht nur das, sondern auch die Logik dahinter. Und das auf eine Art und Weise, die kindgerecht und verständlich ist. 

Persönliche Meinung:

Ich finde es wichtig, dass die Kinder die Konzepte verstehen, bevor sie die Syntax lernen. Das ist der Schlüssel zum Erfolg. Wenn die Kinder die Konzepte verstehen, dann können sie auch die Syntax verstehen. Wenn die Kinder die Syntax verstehen, dann können sie auch die Konzepte verstehen. Es ist ein Teufelskreis, aber ein guter Teufelskreis.
Sie Lernen Nativ MCS Schreiben auf eine Art die Verständlich ist und die sie auch später in der echten (anderen)Programmierwelten brauchen werden!