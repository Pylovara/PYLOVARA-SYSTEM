# Vorwort: Willkommen in der Welt der Logik

Bisher haben wir das Lernmaterial für Kinder im Kindergarten- und 
Grundschulalter so aufbereitet, dass sie die Grundlagen 
der Pylovara-Sprache spielerisch entdecken konnten. 

Ab hier machen wir den nächsten Schritt. 
Das Material für die Klassenstufen 1 bis 4 (und darüber hinaus) bereitet 
den Übergang von der "Magie" zur echten technischen Struktur vor. 
Idealerweise sollte das Kind das Material bereits eigenständig lesen 
und die Konzepte dahinter erfassen können.

Deshalb führen wir ab dieser Stufe die professionellen Fachbegriffe ein, 
die wir zuvor bildlich umschrieben haben:

- **Argumente** (Zauber-Karusell «« mit Loop als Wiederholungszauber)
- **Sentiatoren** (Zustandsprüfer/Entscheidungslogik)
- **Transaktionsrahmen** (statt Zauberrand)
- **Arbeitsflächenprozess** (statt Meister REX)
- **Arbeitsflächenkoordination** (Struktur der Workspaces)
- **Protein** (statt Zauberkiste)
- **Proton** (statt kleine Zauberkiste)
- **Aktionsdraht** (statt Aktionszauber)

Jedoch als Zweitmerkmal und das Lernmaterial übergreifend,
wir bleiben bei den Zauberbegriffen und den Fachbegriffen,
so das die Kinder beides verstehen können und in den 
(sogenannten) „Eselsbrücken Gedächtnis“ hängen bleibt.
(dazu am schluss noch mehr)

Darüber hinaus führen wir das Konzept:

- der **Datenstrukturierung** 
- und **Zustandsverwaltung** ein. 

Wir erweitern die abstrakten Konzepte von Stufe 1 
(Transaktionsrahmen, Protein, Arbeitsfläche, REX) 
um die praktische Anwendung in der digitalen Welt verbessert 
zu vermitteln.

## Die neuen Konzepte im Überblick:

### 1. Das Proton `{}` (Kleine Zauberkiste)

- **Abstraktion:** 
Ein Container für spezifische Werte oder Zustände.

- **Analogie:** 
Während die große Kiste `[]` schlafen oder wach sein kann,
das dass Objekt selbst ist, enthält die kleine Kiste `{}` 
dessen Eigenschaften (Attribute) **Launchbox|KleineKiste** .

Hierbei handelt es sich um :

- ausführbares Protein
- nichtausführbares Protein
- Hardware Proton versorgung, sprich:
    - Strom (HauptElement)
    - Kühlung (HauptElement)
    - Kühlflüssigkeit/Lüftergeschwindigkeit (HauptElement)
    und des gleichen (PCIE, RAM, CPU, GPU, etc. Strom, Kühlung, 
    Kühlflüssigkeit/Lüftergeschwindigkeit)

- **Technische Relevanz:** 
Entspricht der Definition von 
**Objektattributen** 
oder 
**Konfigurationsparametern** 
in der Programmierung.

### 2. Schlafende vs. Wache Kisten

- **Schlafend:** (nicht ausführbar)
Daten liegen bereit, aber es passiert nichts. 
(Statische Daten)

- **Wach:** (ausführbar)
Daten werden aktiv verarbeitet. 
(Dynamische Daten / Execution)
- **Technisch:** 
Dies simuliert den Unterschied zwischen 
**Datenspeicherung** (RAM/Festplatte) und 
**Datenverarbeitung** (CPU/Execution Thread).

## Didaktischer Ansatz
Wir nutzen weiterhin die **bildliche Metapher**, um komplexe technische 
Sachverhalte greifbar zu machen. Das Kind lernt nicht „Syntax“, sondern 
„Logik“. Indem es dem Hund sein Futter in eine kleine Kiste packt, 
versteht es intuitiv das Prinzip von **Objekt + Zustand**. Vermitteln 
Sie dabei spielerisch, dass die Namen der Kisten zweitrangig sind – 
entscheidend für die Funktion sind die Symbole `{}` für die kleine 
und `[]` für die große Kiste und der bezug zu echten Namen wie
**ausfürbares Protein** und **nicht ausführbares Protein**.

## Wichtige Hinweise für die Begleitung
- **Geduld:** 
Die Unterscheidung zwischen `[]` (Container) und `{}` (Inhalt/Zustand) 
kann anfangs ungewohnt sein. Üben Sie gemeinsam das „Packen“ und 
„Auspacken“.

- **Kreativität fördern:** 
Ermutigen Sie das Kind, eigene „Lunchboxen“ für seine Spielfiguren 
zu erfinden, hierbei ist es nicht wichtig ob der Hund Pizza ist oder 
Knochen, wichtig ist das die Logik dahinter verstanden wird.

- **Nutzen sie Meister REX** 
um die Arbeitssflächen zu vermitteln und die Logik dahinter zu verstehen,
ist es ratsam dem Kind immer mitzuteilen : 
„Wir brauchen Für jede Kiste einen Arbeitstisch , damit wir wissen was 
wir tun können“

## Ratsam und Empfehlenswert:
- **Tastatur:** Helfen Sie beim Finden der Klammertasten auf der 
Tastatur. Dies ist die erste praktische Berührung mit „echter“ 
Programmier-Syntax.

In diesem Lernmaterial werden wir die Symbole der MCS Struktur 
statt mit Stift und Papier am Computer erarbeiten. 

Sie können natürlich auch auf Blätter umstellen, wenn sie das möchten
- jedoch hat das den Nachteil das sie nicht so schnell korrigieren können
  und das die Kinder nicht das tippen lernen.

Wenn sie sich für die Computer Variante entscheiden, empfehle ich ihnen 
ein Schreibprogramm zu nutzen, das die Klammern farblich hervorhebt. 
Das hilft den Kindern enorm bei der Orientierung.
(Kleiner tipp am Rande von mir als ArchLinux User: Kate|VS Code|LibreOffice Writer)
Sie können somit am Bildschirm LIVE ohne Radiergummis jede Existierende
MCS Struktur ausbauen und erweitern und gleichzeitig lernt ihr Kind:
-Tippen
-Tastatur Möglichkeiten
-nebensächlich das Schreiben
Achten sie Darauf das sie UTF -8 Deutscher Standart verwenden den
manche Ländereinstellungen UTF-Versionen können mit den 
Sonderzeichen nicht umgehen, die aber Elementar sind.


Damit legen wir den Grundstein für echtes logisches Denken und 
bereiten die Kinder spielerisch auf die Welt der Programmierung vor. 
Es geht dabei weniger um das bloße Auswendiglernen von Zeichenfolgen, 
sondern vielmehr um das tiefe Verständnis von Strukturen und Abläufen. 
Die Symbole dienen als Brücke, um diese abstrakten Konzepte begreifbar 
zu machen – eine Wissensbasis, die den Weg für spätere native 
Softwareentwicklung (bis hin zu klassischen Umgebungen wie C, GCC oder 
GNU) ebnet.

wir arbeiten von unseren Standpunkt ausgesehen rückwärts, 
das heißt wir bringen den Kindern die Konzepte bei und nicht die Syntax. 
Die Syntax kommt später ganz von alleine. 

Beispiel für eine erweiterte Kaffemaschinen Einsatz nahen Erklärung:

`KAFFEESTARTER.KAFFEECREMAMASCHINE-CORE` 
# In der Datei Aktualisieren wir dann:

¢!

Ŧ¹ »["KNOPF AN"|{Volt:230, Watt:1000, Kapazität:50.0L}]« # 50 liter wirds leider nicht geben ;-) 
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
--------------------------------------------------------------
Kurz Info zu Sentiator-regel-der-reinheit :

ſ »["Kaffee ist nicht fertig, reinige aber den Speicher!"]«

das lehrt die Kinder : "Hinterlasse niemals Datenmüll"
--------------------------------------------------------------

Sie verstehen also das ihr in diesem Punkt die Syntax der MCS-Sprache 
lernt - aber nicht nur das, sondern auch die Logik dahinter. 
Auf diese Art und Weise, Lernen wir die kindgerecht und verständlich 
art des vorgehens. 

Wir führen auch ab dieser Phase , neber den Zauber* begriffen , die 
echten Fachbegriffe ein vom Pylovara-System.

Und keine Angst, wir werden die Kinder nicht mit den Fachbegriffen 
überfordern. Wir führen sie langsam und schrittweise ein - Und
das Kaffemaschinen Beispiel dient lediglich für ihr eigenes 
Verständnisses, was ihr Kind hier eigentlich Lernt!
Sie können sich ja zum spaß über KI mit dem Kaffemaschinen Beispiel
zeigen lassen , wieviel Fachbegriffe da eigentlich drin stecken -
oder sich zeigen lassen was in C geschrieben eigentlich bedeutet.

Ich Garantiere ihnen mit Brief und Siegel : „Sie werden Große Augen 
machen, wenn sie sehen das sie nichts verstehen - was aber die 
Kaffemaschine ihnen jedoch verständlich erklärt hat“
Am besten sie Copieren den kaffemaschinen code in einen LLM Chat
mit der zusatzs frage „erklär mir mal wieviel code es braucht für sowas triviales wie eine Kaffeemaschine in C oder python oder sonnst was“

Am besten haben sie genug RAM den die LLM wird vor lauter Zeilen
vermutlich abstürzen oder nur einen Bruchteil erklären können.

ich bin der Meinung :

Das wenn sie nichts Verstehen , dann auch ihr (oder ihre schützlinge)
ebenfalls nicht - und wenn sie den Sinn verstehen, ist die erfolgschance
das ihr Kind dabei Spaß hat und es versteht extrem hoch.

Ich finde es wichtig, dass die Kinder die Konzepte verstehen, bevor sie 
die Syntax lernen. Das ist der Schlüssel zum Erfolg. Wenn die Kinder die 
Konzepte verstehen, dann können sie auch die Syntax verstehen. Wenn die 
Kinder die Syntax verstehen, dann können sie auch die Konzepte 
verstehen. Es ist ein Teufelskreis, aber ein guter Teufelskreis.
Sie Lernen Nativ MCS Schreiben auf eine Art die Verständlich ist und die 
sie auch später brauchen werden!

Denken sie am besten Immer an das hier :

„Sie bringen nicht nur ihrem Kind was bei , sondern auch sich selbst“
„Den sie sind der Motor der gegenwart und ihr Kind der Motor der zukunft“


Gedanklicher Zusatz:

ich persönlich wäre Froh gewesen, wenn mein Vater mit so einem 
Lehrgang um die ecke gekommen wäre - vielleicht nicht Akut als Kind,
aber spätestens wenn ich als Erwachsener begriffen hätte was das hier
mir persönlich für Türen geöffnet hat, hätte ich mich spätestens da 
in den Arsch gebissen vor Freude!


---

Mit freundlichen Grüßen,

**Thomas Zimmermann**  
*(Pylovara-System-Entwickler)*  
GoogleSearch: Pylovara System Thomas Zimmermann