// =============================================================================
// include/mcs_07_boxis.h
// Pylovara MCS Kernel – Modul 07: Boxis
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Sicherer Ausführungs- und Isolationsraum
// =============================================================================

#ifndef MCS_07_BOXIS_H
#define MCS_07_BOXIS_H

#include "mcs_01_datentypen.h"           // B64, B1
#include "mcs_03_protein.h"              // mcs_protein_t
#include "mcs_04_proton.h"               // mcs_proton_t
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Boxis – Grunddefinition
// =============================================================================

// Boxis sind abgeschottete, kontrollierte Räume im Pylovara-System.
// Sie dienen als Sandbox, Ausführungscontainer oder geschützter Schaltkreis.
// Der Zweck einer Boxis ist es, unkontrollierten Signalfluss (Protonen-Schub)
// nach außen zu verhindern und definierte, sichere Ein- und Ausgänge
// zu ermöglichen – später über Needles oder Master-Control.
// Boxis sind der **sichere Ort**, an dem Aktionen stattfinden, ohne das
// Gesamtsystem zu gefährden. Sie schützen vor Überlastung, Fehlverhalten
// und unbefugtem Zugriff – unverzichtbar für Systemintegrität.

typedef enum {
    BOXIS_SANDBOX = 0,              // Voll isolierte Testumgebung – kein Außenkontakt
    BOXIS_EXEC = 1,                 // Ausführungsbox – kontrollierte Programmausführung
    BOXIS_ISOLATION = 2,            // Reine Haltebox – Inhalt eingefroren
    BOXIS_WARP_GATE = 3,            // Warp-Ein-/Ausgangsbox – Hochgeschwindigkeitskanal
    BOXIS_FEED_BUFFER = 4,          // Zwischenspeicher für Feed-Ketten
    BOXIS_SENSOR = 5                // Beobachtungsbox – nur Lesen, kein Schreiben
} boxis_typ_t;

// Status einer Boxis – bestimmt den aktuellen Betriebszustand
typedef enum {
    BOXIS_LEER = 0,                 // Kein Inhalt vorhanden
    BOXIS_BELEGT = 1,               // Enthält Protein(e) und/oder Proton(en)
    BOXIS_AKTIV = 2,                // Ausführung oder Verarbeitung läuft
    BOXIS_GESPERRT = 3              // Durch Master-Control oder Trennbefehl blockiert
} boxis_status_t;

// =============================================================================
// Hauptstruktur einer Boxis
// =============================================================================

typedef struct mcs_boxis {
    // Identifikation und Typ
    B64                 boxis_id;           // Eindeutige System-ID
    boxis_typ_t         typ;

    // Aktueller Status
    boxis_status_t      status;

    // Inhalt der Box – verkettete Listen
    struct mcs_protein* protein_kopf;       // Erstes Protein in der Box
    struct mcs_proton*  proton_kopf;        // Erstes Proton in der Box
    B64                 inhalt_anzahl;      // Gesamtanzahl Objekte

    // Ein-/Ausgangskontrolle – Sicherheitsventile
    B1                  eingang_offen;      // 1 = Neue Elemente dürfen eintreten
    B1                  ausgang_offen;      // 1 = Ergebnisse dürfen austreten
    B64                 max_kapazitaet;     // Maximale Objektanzahl (Überlaufschutz)

    // Scheduling und Synchronisation
    B64                 sync_timer;         // Verknüpfung mit Magic-Timer (Zyklus-Referenz)
    B64                 priority;           // Priorität bei Ressourcenzuweisung
    B64                 zugriffs_node;      // Welcher Master-Control-Node hat Zugriff

    // Metadaten für Nachvollziehbarkeit
    B64                 erstellungs_zeit;   // Wann wurde die Boxis erzeugt
    B64                 letzte_aktion;      // Zeitpunkt der letzten Operation

    // Reserviert für zukünftige Erweiterungen (z. B. Needle-Ports, AI-Überwachung)
    B64                 reserved[6];

    // Verkettung aller Boxis im System – für globale Master-Control-Übersicht
    struct mcs_boxis*   next_boxis;
    struct mcs_boxis*   prev_boxis;

} mcs_boxis_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_BOXIS_NULL { \
.boxis_id           = 0, \
.typ                = BOXIS_SANDBOX, \
.status             = BOXIS_LEER, \
.protein_kopf       = NULL, \
.proton_kopf        = NULL, \
.inhalt_anzahl      = 0, \
.eingang_offen      = 0, \
.ausgang_offen      = 0, \
.max_kapazitaet     = 100, \
.sync_timer         = 0, \
.priority           = 0, \
.zugriffs_node      = 0, \
.erstellungs_zeit   = 0, \
.letzte_aktion      = 0, \
.reserved           = {0}, \
.next_boxis         = NULL, \
.prev_boxis         = NULL \
}

// Beispiel: Standard-Ausführungsbox mit offenen Toren
#define MCS_BOXIS_EXEC_STANDARD { \
.boxis_id           = 0, /* wird vom System vergeben */ \
.typ                = BOXIS_EXEC, \
.status             = BOXIS_LEER, \
.protein_kopf       = NULL, \
.proton_kopf        = NULL, \
.inhalt_anzahl      = 0, \
.eingang_offen      = 1, \
.ausgang_offen      = 1, \
.max_kapazitaet     = 1000, \
.sync_timer         = 0, \
.priority           = 50, \
.zugriffs_node      = 0, \
.erstellungs_zeit   = 0, \
.letzte_aktion      = 0, \
.reserved           = {0}, \
.next_boxis         = NULL, \
.prev_boxis         = NULL \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_boxis_ist_leer(const mcs_boxis_t* b) {
    return b && (b->status == BOXIS_LEER || b->inhalt_anzahl == 0);
}

static inline bool mcs_boxis_ist_aktiv(const mcs_boxis_t* b) {
    return b && b->status == BOXIS_AKTIV;
}

static inline bool mcs_boxis_eingang_erlaubt(const mcs_boxis_t* b) {
    return b && b->eingang_offen == 1 && b->status != BOXIS_GESPERRT;
}

static inline bool mcs_boxis_ausgang_erlaubt(const mcs_boxis_t* b) {
    return b && b->ausgang_offen == 1 && b->status != BOXIS_GESPERRT;
}

static inline bool mcs_boxis_ist_sandbox(const mcs_boxis_t* b) {
    return b && b->typ == BOXIS_SANDBOX;
}

static inline bool mcs_boxis_ist_warp_gate(const mcs_boxis_t* b) {
    return b && b->typ == BOXIS_WARP_GATE;
}

// =============================================================================
// Ende Modul 07
// =============================================================================

#endif // MCS_07_BOXIS_H
