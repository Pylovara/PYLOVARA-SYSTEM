// =============================================================================
// include/mcs_04_proton.h
// Pylovara MCS Kernel – Modul 04: Protonen
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Signal- und Zustandsträger des Systems
// =============================================================================

#ifndef MCS_04_PROTON_H
#define MCS_04_PROTON_H

#include "mcs_01_datentypen.h"           // B64
#include "mcs_03_protein.h"              // mcs_protein_t – Proton bindet an Protein
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Proton – Grunddefinition
// =============================================================================

// Protonen sind die elementaren aktiven Komponenten des Pylovara-Systems.
// Sie repräsentieren den Impuls, die Ladung, den Schub oder den Zustand,
// der ein Protein antreibt oder durch das System transportiert wird.
// Im Gegensatz zum passiven Protein (Daten) ist das Proton der **aktive Träger**
// von Energie und Signal – bio-inspiriert und photonisch erweiterbar.
// Protonen können frei fließen, an Proteine gebunden sein oder in Warp-Kanälen
// hochgeschwindigkeits-transportiert werden.

typedef enum {
    PROTON_FREI = 0,             // Ungebunden – freier Signalimpuls im System
    PROTON_GEBUNDEN = 1,         // Fest an ein Protein gebunden (Standardzustand)
    PROTON_WARP = 2              // In einem Warp-Kanal (photonischer Hochgeschwindigkeitstransport)
} proton_zustand_t;

// Typ des Signalvektors – bestimmt die Funktion des Protons
typedef enum {
    PROTON_IMPULS = 0,           // Einfacher Schaltimpuls (B1-ähnlich, z. B. ON/OFF)
    PROTON_DATENSTROM = 1,       // Trägt komplexe Payload (Daten, Befehle)
    PROTON_STEUER = 2,           // Steuersignal von Master-Control oder Needle
    PROTON_SENSOR = 3            // Von Sentiator erzeugt (Eingabe aus realer Welt)
} proton_vektor_typ_t;

// =============================================================================
// Hauptstruktur eines Protons
// =============================================================================

typedef struct mcs_proton {
    // Zustand und Signaltyp
    proton_zustand_t    zustand;
    proton_vektor_typ_t vektor_typ;

    // Signalstärke / Ladung
    // Repräsentiert Intensität, Spannung oder photonische Amplitude
    B64                 ladung;

    // Optionale Payload für komplexe Protonen
    B64                 payload_size;       // 0 = kein Payload
    void*               payload;            // Dynamisch allokiert

    // Bindung an ein Protein
    struct mcs_protein* gebundenes_protein; // NULL bei freiem oder Warp-Proton

    // Verkettung – für Feeds, Warps oder Ringstrukturen (z. B. 8-Takt-Lichtkreis)
    struct mcs_proton*  next_proton;
    struct mcs_proton*  prev_proton;

    // Metadaten für Scheduling und Photonik
    B64                 timestamp;          // Erzeugungs- oder Einschaltzeitpunkt
    B64                 priority;           // Priorität im Schub (Magic/Node-gesteuert)
    B64                 warp_channel;       // Kanal-ID bei PROTON_WARP
    B64                 reserved[4];        // z. B. für Phase in Lichtkreis, Frequenz, Polarisation

} mcs_proton_t;

// =============================================================================
// Standard-Initialisierungen
// =============================================================================

#define MCS_PROTON_NULL { \
.zustand             = PROTON_FREI, \
.vektor_typ          = PROTON_IMPULS, \
.ladung              = 0, \
.payload_size        = 0, \
.payload             = NULL, \
.gebundenes_protein  = NULL, \
.next_proton         = NULL, \
.prev_proton         = NULL, \
.timestamp           = 0, \
.priority            = 0, \
.warp_channel        = 0, \
.reserved            = {0} \
}

// Beispiel: Einfacher eingeschalteter Impuls (wie B1_ON)
#define MCS_PROTON_IMPULS_EIN { \
.zustand             = PROTON_FREI, \
.vektor_typ          = PROTON_IMPULS, \
.ladung              = 1, \
.payload_size        = 0, \
.payload             = NULL, \
.gebundenes_protein  = NULL, \
.next_proton         = NULL, \
.prev_proton         = NULL, \
.timestamp           = 0, \
.priority            = 50, \
.warp_channel        = 0, \
.reserved            = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_proton_ist_eingeschaltet(const mcs_proton_t* p) {
    return p && p->ladung != 0;
}

static inline bool mcs_proton_ist_gebunden(const mcs_proton_t* p) {
    return p && p->zustand == PROTON_GEBUNDEN && p->gebundenes_protein != NULL;
}

static inline bool mcs_proton_ist_im_warp(const mcs_proton_t* p) {
    return p && p->zustand == PROTON_WARP;
}

static inline bool mcs_proton_hat_payload(const mcs_proton_t* p) {
    return p && p->payload_size > 0 && p->payload != NULL;
}

static inline bool mcs_proton_ist_sensorimpuls(const mcs_proton_t* p) {
    return p && p->vektor_typ == PROTON_SENSOR;
}

// =============================================================================
// Ende Modul 04
// =============================================================================

#endif // MCS_04_PROTON_H
