// =============================================================================
// include/mcs_05_trennbefehl.h
// Pylovara MCS Kernel – Modul 05: Trennbefehl
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Isolations-, Reinigungs- und Sicherheitsmechanismus
// =============================================================================

#ifndef MCS_05_TRENNBEFEHL_H
#define MCS_05_TRENNBEFEHL_H

#include "mcs_01_datentypen.h"           // B1, B64
#include "mcs_03_protein.h"              // mcs_protein_t
#include "mcs_04_proton.h"               // mcs_proton_t
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Trennbefehl – Grunddefinition
// =============================================================================

// Der Trennbefehl ist der zentrale Sicherheits- und Reinigungsmechanismus
// des Pylovara-Systems. Er ermöglicht die kontrollierte Trennung von
// Signalflüssen (Protonen) und Datenträgern (Proteinen), ohne den
// Transaktionsrahmen oder das Gesamtsystem zu beschädigen.
// Er wird eingesetzt, um:
// - Ressourcen freizugeben
// - Speicherbereiche (Feeds, Cache, Warps) zu säubern
// - Unerwünschte Interaktionen zu verhindern
// - Schaltkreise vor Überlastung zu schützen
// Der Trennbefehl ist ein unverzichtbarer Bestandteil der Systemintegrität.

typedef enum {
    TRENNBEFEHL_ISOLIEREN = 0,              // Trennt Proton von Protein – beide bleiben intakt
    TRENNBEFEHL_REINIGEN = 1,               // Setzt Zustand auf Null (ALU_REINIGEN-Äquivalent)
    TRENNBEFEHL_FREIGEBEN = 2,              // Gibt Speicher und Bindungen frei
    TRENNBEFEHL_BLOCKIEREN = 3,             // Verhindert weiteren Schub/Impact (Sicherheitsstop)
    TRENNBEFEHL_VERKETTUNG_AUFLOESEN = 4    // Lößt next/prev-Verkettungen in Ketten auf
} trennbefehl_typ_t;

// Ziel des Trennbefehls – bestimmt, worauf der Befehl angewendet wird
typedef enum {
    TRENN_ZIEL_PROTEIN = 0,                 // Einzelnes Protein
    TRENN_ZIEL_PROTON = 1,                  // Einzelnes Proton
    TRENN_ZIEL_KETTE = 2,                   // Gesamte Protein- oder Protonenkette
    TRENN_ZIEL_RAHMEN = 3                   // Kompletter Transaktionsrahmen (zukunftig)
} trennbefehl_ziel_t;

// =============================================================================
// Hauptstruktur eines Trennbefehls
// =============================================================================

typedef struct mcs_trennbefehl {
    // Art des Befehls
    trennbefehl_typ_t   typ;

    // Zielkategorie
    trennbefehl_ziel_t  ziel;

    // Direkte Referenz auf das Zielobjekt (union für Speichereffizienz)
    union {
        mcs_protein_t*  protein;
        mcs_proton_t*   proton;
        void*           kette_kopf;         // Zeiger auf ersten Eintrag einer Kette
        void*           rahmen;             // Reserviert für Rahmenbezug
    } ziel_objekt;

    // Optionale Parameter
    B64                 sektor_id;          // z. B. Feed-Cache-Sektor oder Warp-Kanal
    B64                 intensitaet;        // Stärke der Reinigung (0 = soft, max = hard reset)
    B1                  sofort_ausfuehren;  // 1 = sofort, 0 = in Queue (Master-Control)

    // Ausführungsmetadaten
    B64                 timestamp;          // Wann wurde der Befehl erzeugt
    B64                 ausfuehrer_node;    // Welcher Master-Control-Node hat befohlen
    B64                 reserved[5];        // Zukunftssicherheit (z. B. Signatur, AI-Flag)

} mcs_trennbefehl_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_TRENNBEFEHL_NULL { \
.typ               = TRENNBEFEHL_ISOLIEREN, \
.ziel              = TRENN_ZIEL_PROTEIN, \
.ziel_objekt       = { .protein = NULL }, \
.sektor_id         = 0, \
.intensitaet       = 0, \
.sofort_ausfuehren = 0, \
.timestamp         = 0, \
.ausfuehrer_node   = 0, \
.reserved          = {0} \
}

// Beispiel: Sofortige Reinigung eines Protons
#define MCS_TRENNBEFEHL_REINIGEN_PROTON(proton_ptr) { \
.typ               = TRENNBEFEHL_REINIGEN, \
.ziel              = TRENN_ZIEL_PROTON, \
.ziel_objekt       = { .proton = (proton_ptr) }, \
.sektor_id         = 0, \
.intensitaet       = 100, \
.sofort_ausfuehren = 1, \
.timestamp         = 0, \
.ausfuehrer_node   = 0, \
.reserved          = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_trennbefehl_ist_reinigung(const mcs_trennbefehl_t* tb) {
    return tb && tb->typ == TRENNBEFEHL_REINIGEN;
}

static inline bool mcs_trennbefehl_zielt_auf_proton(const mcs_trennbefehl_t* tb) {
    return tb && tb->ziel == TRENN_ZIEL_PROTON;
}

static inline bool mcs_trennbefehl_ist_sofort(const mcs_trennbefehl_t* tb) {
    return tb && tb->sofort_ausfuehren == 1;
}

static inline bool mcs_trennbefehl_ist_blockade(const mcs_trennbefehl_t* tb) {
    return tb && tb->typ == TRENNBEFEHL_BLOCKIEREN;
}

// =============================================================================
// Ende Modul 05
// =============================================================================

#endif // MCS_05_TRENNBEFEHL_H
