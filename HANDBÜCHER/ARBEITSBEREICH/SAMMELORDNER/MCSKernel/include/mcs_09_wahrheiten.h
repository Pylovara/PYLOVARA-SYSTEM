// =============================================================================
// include/mcs_09_wahrheiten.h
// Pylovara MCS Kernel – Modul 09: Wahrheiten
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Grundlegende logische Zustände und Wahrheitsträger
// =============================================================================

#ifndef MCS_09_WAHRHEITEN_H
#define MCS_09_WAHRHEITEN_H

#include "mcs_01_datentypen.h"           // B64, B1
#include "mcs_04_proton.h"               // mcs_proton_t – Proton als Träger
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Wahrheiten – Grunddefinition
// =============================================================================

// Wahrheiten sind die fundamentalen, unveränderlichen logischen Zustände
// des Pylovara-Systems. Sie bilden die Basis jeder Entscheidung, jedes Arguments
// und jeder projektiven Rechenlogik.
// Der Kernwert ist immer B1-basiert (ON/OFF), getragen durch Protonen oder
// direkt in physischen Gattern des BMC.
// Eine Wahrheit ist im Moment ihrer Festlegung immutable und kann nur durch
// kontrollierte Aktionen (z. B. SCHUB, SYNC, Master-Control) verändert werden.
// Dies gewährleistet Integrität und Verlässlichkeit – das System basiert auf Wahrheit.

typedef enum {
    WAHRHEIT_FALSCH = 0,            // B1_OFF – Kein Impuls, kein Schaltzustand
    WAHRHEIT_WAHR   = 1             // B1_ON  – Impuls vorhanden, Schalter aktiv
} wahrheit_wert_t;

// Erweiterte Zustände – für komplexe, reale Systemdynamik
typedef enum {
    WAHRHEIT_DEFINITIV_FALSCH = 0,  // Endgültig falsch
    WAHRHEIT_DEFINITIV_WAHR   = 1,  // Endgültig wahr
    WAHRHEIT_UNBESTIMMT       = 2,  // z. B. während Warp, Sync oder Messung
    WAHRHEIT_KONFLIKT         = 3   // Zwei gegensätzliche Protonen im selben Gatter
} wahrheit_erweitert_t;

// =============================================================================
// Hauptstruktur einer Wahrheit
// =============================================================================

typedef struct mcs_wahrheit {
    // Kernwert – immer B1-basiert
    wahrheit_wert_t     wert;

    // Erweiterter Zustand – für Debugging und Master-Control
    wahrheit_erweitert_t erweitert;

    // Träger der Wahrheit
    union {
        struct mcs_proton* proton;          // Durch Proton transportiert
        B64                gatter_adresse;  // Direkte physische Gatter-Referenz
        B64                konstante_wert;   // Kompilierte Konstante
    } traeger;

    // Herkunft und zeitliche Gültigkeit
    B64                 erzeugungs_timestamp;
    B64                 gueltig_bis;            // 0xFFFFFFFFFFFFFFFF = unendlich
    B64                 quelle_node;            // Welcher Master-Control-Node hat gesetzt

    // Sicherheit und Integrität
    B1                  immutable;              // 1 = Kann nicht mehr geändert werden
    B64                 signatur_hash;          // Kryptogenetische Prüfsumme

    // Verkettung – für Argumente, Feeds oder logische Ketten
    struct mcs_wahrheit* next_wahrheit;
    struct mcs_wahrheit* prev_wahrheit;

    // Zukunftssicherheit
    B64                 reserved[4];

} mcs_wahrheit_t;

// =============================================================================
// Standard-Wahrheiten (Konstanten)
// =============================================================================

#define MCS_WAHRHEIT_FALSCH_KONST { \
.wert               = WAHRHEIT_FALSCH, \
.erweitert          = WAHRHEIT_DEFINITIV_FALSCH, \
.traeger            = { .konstante_wert = 0 }, \
.erzeugungs_timestamp = 0, \
.gueltig_bis        = 0xFFFFFFFFFFFFFFFF, \
.quelle_node        = 0, \
.immutable          = 1, \
.signatur_hash      = 0, \
.next_wahrheit      = NULL, \
.prev_wahrheit      = NULL, \
.reserved           = {0} \
}

#define MCS_WAHRHEIT_WAHR_KONST { \
.wert               = WAHRHEIT_WAHR, \
.erweitert          = WAHRHEIT_DEFINITIV_WAHR, \
.traeger            = { .konstante_wert = 1 }, \
.erzeugungs_timestamp = 0, \
.gueltig_bis        = 0xFFFFFFFFFFFFFFFF, \
.quelle_node        = 0, \
.immutable          = 1, \
.signatur_hash      = 0, \
.next_wahrheit      = NULL, \
.prev_wahrheit      = NULL, \
.reserved           = {0} \
}

#define MCS_WAHRHEIT_UNBESTIMMT { \
.wert               = WAHRHEIT_FALSCH, \
.erweitert          = WAHRHEIT_UNBESTIMMT, \
.traeger            = { .proton = NULL }, \
.erzeugungs_timestamp = 0, \
.gueltig_bis        = 0, \
.quelle_node        = 0, \
.immutable          = 0, \
.signatur_hash      = 0, \
.next_wahrheit      = NULL, \
.prev_wahrheit      = NULL, \
.reserved           = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_wahrheit_ist_definitiv_wahr(const mcs_wahrheit_t* w) {
    return w && w->wert == WAHRHEIT_WAHR && w->erweitert == WAHRHEIT_DEFINITIV_WAHR;
}

static inline bool mcs_wahrheit_ist_definitiv_falsch(const mcs_wahrheit_t* w) {
    return w && w->wert == WAHRHEIT_FALSCH && w->erweitert == WAHRHEIT_DEFINITIV_FALSCH;
}

static inline bool mcs_wahrheit_ist_unbestimmt(const mcs_wahrheit_t* w) {
    return w && w->erweitert == WAHRHEIT_UNBESTIMMT;
}

static inline bool mcs_wahrheit_hat_konflikt(const mcs_wahrheit_t* w) {
    return w && w->erweitert == WAHRHEIT_KONFLIKT;
}

static inline bool mcs_wahrheit_ist_immutable(const mcs_wahrheit_t* w) {
    return w && w->immutable == 1;
}

static inline bool mcs_wahrheit_ist_gueltig(const mcs_wahrheit_t* w, B64 aktueller_zeitpunkt) {
    return w && (w->gueltig_bis == 0xFFFFFFFFFFFFFFFF || w->gueltig_bis >= aktueller_zeitpunkt);
}

// =============================================================================
// Ende Modul 09
// =============================================================================

#endif // MCS_09_WAHRHEITEN_H
