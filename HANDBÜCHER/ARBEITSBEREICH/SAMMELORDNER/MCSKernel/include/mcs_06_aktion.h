// =============================================================================
// include/mcs_06_aktion.h
// Pylovara MCS Kernel – Modul 06: Aktionen
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Aktive Operationen und Befehlsausführung
// =============================================================================

#ifndef MCS_06_AKTION_H
#define MCS_06_AKTION_H

#include "mcs_01_datentypen.h"           // B64, B1
#include "mcs_03_protein.h"              // mcs_protein_t
#include "mcs_04_proton.h"               // mcs_proton_t
#include "mcs_05_trennbefehl.h"          // mcs_trennbefehl_t (für LÖSEN)
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Aktion – Grunddefinition
// =============================================================================

// Aktionen sind die aktiven, ausführenden Operationen des Pylovara-Systems.
// Sie bilden den entscheidenden "Draht" zwischen Daten (Protein) und Energie (Proton).
// Eine Aktion verbindet, steuert, transportiert und transformiert – sie ist der
// Mechanismus, durch den das System handelt.
// Aktionen werden durch Argumente, Sentiatoren oder Master-Control ausgelöst
// und interagieren mit Boxis, Feeds, Warps und dem Sync-Timer.

typedef enum {
    AKTION_AUSFUEHREN = 0,          // Startet die Ausführung eines Proteins
    AKTION_SCHUB = 1,               // Erzeugt Impact/Schub auf ein Proton (photonisch inspiriert)
    AKTION_BINDEN = 2,              // Bindet ein Proton an ein Protein
    AKTION_LOESEN = 3,              // Löst eine Bindung (kombiniert mit Trennbefehl)
    AKTION_WARP = 4,                // Transportiert ein Proton durch einen Warp-Kanal
    AKTION_FEED = 5,                // Fügt ein Element in einen Feed ein
    AKTION_BOXIS_AUFRUF = 6,        // Ruft eine Funktion innerhalb einer Boxis auf
    AKTION_SYNC = 7,                // Synchronisiert mit dem Magic-Timer
    AKTION_LOG = 8                  // Protokolliert in redflag-notes oder System-Log
} aktion_typ_t;

// =============================================================================
// Hauptstruktur einer Aktion
// =============================================================================

typedef struct mcs_aktion {
    // Art der Aktion
    aktion_typ_t        typ;

    // Quelle der Aktion (was wird bearbeitet)
    union {
        mcs_protein_t*  protein;
        mcs_proton_t*   proton;
        void*           kette_kopf;     // Kopf einer verketteten Struktur
    } quelle;

    // Ziel der Aktion (wohin geht die Wirkung)
    union {
        mcs_protein_t*  ziel_protein;
        mcs_proton_t*   ziel_proton;
        B64             warp_kanal_id;
        B64             feed_id;
        void*           boxis_ptr;
    } ziel;

    // Parameter für Intensität und Timing
    B64                 intensitaet;        // z. B. Stärke des Schubs (0–100)
    B64                 timing_delay;       // Verzögerung in Zyklen (««t-Syntax)
    B1                  sofort;             // 1 = sofort ausführen, 0 = gequeued

    // Metadaten für Tracking und Master-Control
    B64                 aktions_id;         // Eindeutige Laufzeit-ID
    B64                 timestamp;          // Erzeugungszeitpunkt
    B64                 ausfuehrer_node;    // Welcher Master-Control-Node hat ausgelöst
    B64                 reserved[5];        // Zukunftssicherheit (z. B. AI-Flag, Signatur)

} mcs_aktion_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_AKTION_NULL { \
.typ               = AKTION_AUSFUEHREN, \
.quelle            = { .protein = NULL }, \
.ziel              = { .ziel_protein = NULL }, \
.intensitaet       = 0, \
.timing_delay      = 0, \
.sofort            = 0, \
.aktions_id        = 0, \
.timestamp         = 0, \
.ausfuehrer_node   = 0, \
.reserved          = {0} \
}

// Beispiel: Schub auf ein Proton (Lichtpuls-Impact)
#define MCS_AKTION_SCHUB_PROTON(proton_ptr, staerke) { \
.typ               = AKTION_SCHUB, \
.quelle            = { .proton = (proton_ptr) }, \
.ziel              = { .ziel_protein = NULL }, \
.intensitaet       = (staerke), \
.timing_delay      = 0, \
.sofort            = 1, \
.aktions_id        = 0, \
.timestamp         = 0, \
.ausfuehrer_node   = 0, \
.reserved          = {0} \
}

// Beispiel: Ausführung eines Proteins
#define MCS_AKTION_AUSFUEHREN(protein_ptr) { \
.typ               = AKTION_AUSFUEHREN, \
.quelle            = { .protein = (protein_ptr) }, \
.ziel              = { .ziel_protein = NULL }, \
.intensitaet       = 100, \
.timing_delay      = 0, \
.sofort            = 1, \
.aktions_id        = 0, \
.timestamp         = 0, \
.ausfuehrer_node   = 0, \
.reserved          = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_aktion_ist_schub(const mcs_aktion_t* a) {
    return a && a->typ == AKTION_SCHUB;
}

static inline bool mcs_aktion_ist_ausfuehrung(const mcs_aktion_t* a) {
    return a && a->typ == AKTION_AUSFUEHREN;
}

static inline bool mcs_aktion_ist_warp(const mcs_aktion_t* a) {
    return a && a->typ == AKTION_WARP;
}

static inline bool mcs_aktion_ist_sofort(const mcs_aktion_t* a) {
    return a && a->sofort == 1;
}

static inline bool mcs_aktion_hat_quelle_proton(const mcs_aktion_t* a) {
    return a && a->quelle.proton != NULL;
}

// =============================================================================
// Ende Modul 06
// =============================================================================

#endif // MCS_06_AKTION_H
