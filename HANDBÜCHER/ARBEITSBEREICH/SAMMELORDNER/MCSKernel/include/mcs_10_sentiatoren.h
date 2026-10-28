// =============================================================================
// include/mcs_10_sentiatoren.h
// Pylovara MCS Kernel – Modul 10: Sentiatoren
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Sensoren und Beobachter des Systems
// =============================================================================

#ifndef MCS_10_SENTIATOREN_H
#define MCS_10_SENTIATOREN_H

#include "mcs_01_datentypen.h"           // B64
#include "mcs_04_proton.h"               // mcs_proton_t – Sentiator erzeugt Proton
#include "mcs_07_boxis.h"                // mcs_boxis_t – Sentiatoren laufen in SENSOR-Boxis
#include "mcs_09_wahrheiten.h"           // mcs_wahrheit_t – Sentiator erzeugt Wahrheit
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Sentiatoren – Grunddefinition
// =============================================================================

// Sentiatoren sind die primären Eingabequellen und Beobachter des Pylovara-Systems.
// Sie erfassen Zustände aus der realen Welt (Hardware, Umgebung) oder aus
// internen Systemkomponenten und wandeln diese in Protonen (Impuls) und
// Wahrheiten (logischer Zustand) um.
// Sentiatoren sind die "Sinne" des Systems – sie erzeugen den initialen Impuls,
// der dann durch Proteine verarbeitet, durch Aktionen gesteuert und durch
// Master-Control interpretiert wird.
// Sie sind entscheidend für die Reaktivität und Umweltwahrnehmung des Systems.

typedef enum {
    SENTIATOR_HARDWARE = 0,         // Direkter BMC-Gatter-Zugriff (Spannung, Temperatur)
    SENTIATOR_UMGEBUNG = 1,         // Externe Sensoren (Licht, Ton, Tastatur, Netzwerk)
    SENTIATOR_SYSTEM   = 2,         // Interner Zustand (Boxis, Feeds, Warps)
    SENTIATOR_PHOTONIK = 3,         // Lichtintensität, Phase, Frequenz in Lichtkreisen
    SENTIATOR_SOFTWARE = 4          // Abstrakte Beobachtung (Timer, Log, Events)
} sentiator_typ_t;

// Betriebszustand eines Sentiators
typedef enum {
    SENTIATOR_INAKTIV       = 0,
    SENTIATOR_AKTIV         = 1,
    SENTIATOR_FEHLER        = 2,
    SENTIATOR_KALIBRIERUNG  = 3
} sentiator_status_t;

// =============================================================================
// Hauptstruktur eines Sentiators
// =============================================================================

typedef struct mcs_sentiator {
    // Identifikation und Klassifikation
    B64                 sentiator_id;       // Eindeutige System-ID
    sentiator_typ_t     typ;

    // Aktueller Betriebszustand
    sentiator_status_t  status;

    // Letzter erfasster Rohwert
    B64                 rohwert;            // z. B. ADC-Wert, Lichtintensität
    B64                 rohwert_timestamp;  // Zeitpunkt der letzten Messung

    // Umgewandelte Ausgabe
    struct mcs_proton*  erzeugtes_proton;   // Das durch den Sensor erzeugte Proton
    struct mcs_wahrheit* erzeugte_wahrheit; // Die daraus abgeleitete Wahrheit

    // Konfiguration für Schwellwertlogik
    B64                 schwellwert_low;    // Untergrenze für binäre Entscheidung
    B64                 schwellwert_high;   // Obergrenze für binäre Entscheidung
    B64                 abtastrate;         // Abtastungen pro Sync-Timer-Zyklus
    B1                  invertiert;         // 1 = Logik umkehren (z. B. aktives Low)

    // Zuordnung zu sicherer Ausführungsumgebung
    struct mcs_boxis*   zugeordnete_boxis;  // Läuft typischerweise in SENSOR-Boxis

    // Scheduling und Master-Control
    B64                 sync_timer;         // Verknüpfung mit Magic-Timer
    B64                 priority;           // Priorität bei Abtastung
    B64                 zugriffs_node;      // Welcher Node steuert den Sentiator

    // Metadaten für Wartung und Diagnose
    B64                 kalibrierungs_zeit;
    B64                 letzte_abtastung;
    B64                 fehler_counter;

    // Zukunftssicherheit (z. B. Filter, Mittelwertbildung)
    B64                 reserved[5];

    // Verkettung aller Sentiatoren – für globale Master-Control-Übersicht
    struct mcs_sentiator* next_sentiator;
    struct mcs_sentiator* prev_sentiator;

} mcs_sentiator_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_SENTIATOR_NULL { \
.sentiator_id         = 0, \
.typ                  = SENTIATOR_HARDWARE, \
.status               = SENTIATOR_INAKTIV, \
.rohwert              = 0, \
.rohwert_timestamp    = 0, \
.erzeugtes_proton     = NULL, \
.erzeugte_wahrheit    = NULL, \
.schwellwert_low      = 0, \
.schwellwert_high     = 0xFFFFFFFFFFFFFFFF, \
.abtastrate           = 1, \
.invertiert           = 0, \
.zugeordnete_boxis    = NULL, \
.sync_timer           = 0, \
.priority             = 50, \
.zugriffs_node        = 0, \
.kalibrierungs_zeit   = 0, \
.letzte_abtastung     = 0, \
.fehler_counter       = 0, \
.reserved             = {0}, \
.next_sentiator       = NULL, \
.prev_sentiator       = NULL \
}

// Beispiel: Binärer Hardware-Sensor (Schalter)
#define MCS_SENTIATOR_BINÄR_HARDWARE(id, low, high) { \
.sentiator_id         = (id), \
.typ                  = SENTIATOR_HARDWARE, \
.status               = SENTIATOR_AKTIV, \
.rohwert              = 0, \
.rohwert_timestamp    = 0, \
.erzeugtes_proton     = NULL, \
.erzeugte_wahrheit    = NULL, \
.schwellwert_low      = (low), \
.schwellwert_high     = (high), \
.abtastrate           = 1, \
.invertiert           = 0, \
.zugeordnete_boxis    = NULL, \
.sync_timer           = 0, \
.priority             = 80, \
.zugriffs_node        = 0, \
.kalibrierungs_zeit   = 0, \
.letzte_abtastung     = 0, \
.fehler_counter       = 0, \
.reserved             = {0}, \
.next_sentiator       = NULL, \
.prev_sentiator       = NULL \
}

// Beispiel: Photonik-Sentiator für Lichtintensität (8-Takt)
#define MCS_SENTIATOR_PHOTONIK_LICHT { \
.sentiator_id         = 0, /* vom System vergeben */ \
.typ                  = SENTIATOR_PHOTONIK, \
.status               = SENTIATOR_AKTIV, \
.rohwert              = 0, \
.rohwert_timestamp    = 0, \
.erzeugtes_proton     = NULL, \
.erzeugte_wahrheit    = NULL, \
.schwellwert_low      = 1000, \
.schwellwert_high     = 8000, \
.abtastrate           = 8, /* 8-Takt-Abtastung im Lichtkreis */ \
.invertiert           = 0, \
.zugeordnete_boxis    = NULL, \
.sync_timer           = 0, \
.priority             = 90, \
.zugriffs_node        = 0, \
.kalibrierungs_zeit   = 0, \
.letzte_abtastung     = 0, \
.fehler_counter       = 0, \
.reserved             = {0}, \
.next_sentiator       = NULL, \
.prev_sentiator       = NULL \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_sentiator_ist_aktiv(const mcs_sentiator_t* s) {
    return s && s->status == SENTIATOR_AKTIV;
}

static inline bool mcs_sentiator_hat_fehler(const mcs_sentiator_t* s) {
    return s && s->status == SENTIATOR_FEHLER;
}

static inline bool mcs_sentiator_erzeugt_proton(const mcs_sentiator_t* s) {
    return s && s->erzeugtes_proton != NULL;
}

static inline bool mcs_sentiator_erzeugt_wahrheit(const mcs_sentiator_t* s) {
    return s && s->erzeugte_wahrheit != NULL;
}

static inline bool mcs_sentiator_ist_photonik(const mcs_sentiator_t* s) {
    return s && s->typ == SENTIATOR_PHOTONIK;
}

// =============================================================================
// Ende Modul 10
// =============================================================================

#endif // MCS_10_SENTIATOREN_H
