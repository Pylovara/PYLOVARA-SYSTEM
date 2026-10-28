// =============================================================================
// include/mcs_03_protein.h
// Pylovara MCS Kernel – Modul 03: Protein
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Hauptdatenträger des Systems
// =============================================================================

#ifndef MCS_03_PROTEIN_H
#define MCS_03_PROTEIN_H

#include "mcs_01_datentypen.h"           // p_KEY, INIT_V, B64
#include "mcs_02_transaktionsrahmen.h"   // mcs_transaktionsrahmen_t
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Protein – Grunddefinition
// =============================================================================

// Das Protein ist der primäre Datenträger und logische Baustein des Pylovara-Systems.
// Es transportiert sowohl ausführbaren Code als auch reine Daten und ist immer
// in einen Transaktionsrahmen (¢! ... !¢) eingebettet.
// Proteine können durch Protonen angetrieben werden und bilden die Grundlage
// für Feeds, Warps und Master-Control-gesteuerte Prozesse.

typedef enum {
    PROTEIN_AUSFUEHRBAR = 0,     // Direkt ausführbar über MCS-CMD-Register
    PROTEIN_UNAUSFUEHRBAR = 1    // Nur Datenstruktur – wird extern gesteuert
} protein_ausfuehrbarkeit_t;

// =============================================================================
// Hauptstruktur eines Proteins
// =============================================================================

typedef struct mcs_protein {
    // Kryptogenetische Identität und Integrität
    p_KEY                   key;                // p-KEY – variable Signatur

    // Initialisierungsvektor
    INIT_V                  init_vector;        // INIT-V – Startsequenz (mind. 64 Bit)

    // Ausführbarkeitstyp
    protein_ausfuehrbarkeit_t ausfuehrbarkeit;

    // Einbettung in Transaktionsrahmen
    struct mcs_transaktionsrahmen* rahmen;      // Zeiger auf umschließenden Rahmen

    // Payload – eigentliche Nutzdaten
    B64                     payload_size;       // Größe in Bytes
    void*                   payload;            // Dynamischer Block (Code oder Daten)

    // Scheduling und Master-Control-Metadaten
    B64                     priority;           // Prioritätsstufe (von Magic/Node gesteuert)
    B64                     creation_timestamp; // Erzeugungszeitpunkt

    // Reserviert für zukünftige Erweiterungen (z. B. Proton-Bindung, Warp-Status)
    B64                     reserved[6];

    // Verkettung – Proteine können in Feeds oder Warps verkettet sein
    struct mcs_protein*     next_protein;
    struct mcs_protein*     prev_protein;

} mcs_protein_t;

// =============================================================================
// Standard-Initialisierung eines leeren Proteins
// =============================================================================

#define MCS_PROTEIN_NULL { \
.key                = { .length = 0, .data = NULL }, \
.init_vector        = { .header = 0 }, \
.ausfuehrbarkeit    = PROTEIN_UNAUSFUEHRBAR, \
.rahmen             = NULL, \
.payload_size       = 0, \
.payload            = NULL, \
.priority           = 0, \
.creation_timestamp = 0, \
.reserved           = {0}, \
.next_protein       = NULL, \
.prev_protein       = NULL \
}

// =============================================================================
// Inline-Hilfsfunktionen
// Ermöglichen schnelle Statusabfragen ohne Funktionsaufruf-Overhead
// =============================================================================

static inline bool mcs_protein_ist_ausfuehrbar(const mcs_protein_t* p) {
    return p && (p->ausfuehrbarkeit == PROTEIN_AUSFUEHRBAR);
}

static inline bool mcs_protein_hat_gueltige_key(const mcs_protein_t* p) {
    return p && (p->key.length > 0 && p->key.data != NULL);
}

static inline bool mcs_protein_ist_in_transaktionsrahmen(const mcs_protein_t* p) {
    return p && p->rahmen != NULL;
}

// =============================================================================
// Ende Modul 03
// =============================================================================

#endif // MCS_03_PROTEIN_H
