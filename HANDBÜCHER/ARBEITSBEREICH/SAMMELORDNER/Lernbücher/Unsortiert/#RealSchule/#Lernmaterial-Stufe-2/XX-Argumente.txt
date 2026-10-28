# Kapitel 2: Argumente und Zeitsteuerung

Willkommen zur nächsten Herausforderung! 🚀

In der letzten Lektion haben wir das **Proton** `{}` kennengelernt. Wir haben es als "Lunchbox" bezeichnet, aber technisch gesehen ist es der Ort für **Argumente**. 

## Was sind Argumente?
Argumente sind präzise Anweisungen, die wir einem Objekt oder einem Befehl mitgeben. Statt nur zu sagen: "Hund, komm her", nutzen wir das Argument im Proton, um genau zu definieren, *wie* oder *was* passieren soll.

## Neue Symbole verstehen
In komplexen Programmen (Prozess-Strömen) siehst du oft Zeichen am Ende einer Zeile. Diese steuern, wie das System reagiert:

- **«««T [Zahl]**: Das steht für **Takt (Timing)**. Es gibt an, wie lange das System warten soll (wie die Wartezeit beim Kaffeekochen).
- **«««S [Zahl]**: Das steht für **Sensor-Prüfung**. Das System schaut hier nach einem bestimmten Zustand (z.B. "Ist die Tasse da?").

---

## Übung für Fortgeschrittene: Der Maus-Tagesablauf

In diesem Beispiel siehst du, wie Argumente und Zeitsteuerungen zusammenarbeiten:

```text
¢!
»["Maus" | {"Komm her"}]
 ¬· »["Käse" | {"Iss den Käse"}]« # Argument: Was soll die Maus tun?
     ¬· »["Loch" | {"Versteck dich im Loch"}]
     ¬· »["Käse" | {"Iss den Käse"}]
         ¬· ["KurzeRuhe" | {"Schlafenszeit"}]
            # Hier kommt die Zeitsteuerung ins Spiel:
            # S6 bedeutet: Prüfe Sensor-Stufe 6 (Schlaf-Tiefe)
            ¬· »["Schlafenszeit" | {"Schlafenszeit"}]«««S6 
                ¬· »["Aufwachen" | {"Aufwachen"}]«
!¢
```

### Deine Aufgaben:

1. **Analysiere den Code:** Was passiert in der Zeile mit dem "Loch"? Welches Argument bekommt die Maus dort mitgegeben?
2. **Eigene Logik:** Schreibe einen kurzen Ablauf für eine Katze 🐱, die 5 Takte (`T5`) warten muss, bevor sie ihre Milch bekommt.
   *(Tipp: `»["Milch" | {"Trinken"}]«««T5`)*

---

> [!TIP]
> **Für Eltern & Lehrkräfte:** Diese Lektion führt in die **kontrollierte Programmsteuerung** ein. 
> - **Argumente** erlauben die Parametrisierung von Befehlen.
> - **T (Timing)** und **S (Sensors/States)** sind essenziell für die Synchronisation mit der Hardware. `«««S6` prüft beispielsweise einen Registerwert oder einen physischen Sensorzustand, bevor der nächste Prozessschritt eingeleitet wird.
