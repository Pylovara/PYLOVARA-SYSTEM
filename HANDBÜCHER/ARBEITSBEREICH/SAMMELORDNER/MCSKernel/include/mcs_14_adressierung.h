// =============================================================================
// include/mcs_14_adressierung.h
// Pylovara MCS Kernel – Modul 14: Adressierung
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Systemweite Ortung und Zielsteuerung
// =============================================================================

#ifndef MCS_14_ADRESSIERUNG_H
#define MCS_14_ADRESSIERUNG_H

#include "mcs_01_datentypen.h"           // B32, B64
#include "mcs_03_protein.h"              // mcs_protein_t
#include "mcs_04_proton.h"               // mcs_proton_t
#include "mcs_07_boxis.h"                // mcs_boxis_t
#include "mcs_12_feeds.h"                // mcs_feed_t
#include "mcs_13_warps.h"                 // mcs_warp_t
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Adressierung – Grunddefinition
// =============================================================================

// Die Adressierung ist der zentrale Mechanismus zur eindeutigen Lokalisierung
// und Ansteuerung aller Elemente im Pylovara-System.
// Sie vereint physische Gatter-Adressen (BMC), logische Pfade (Nodes, Boxis)
// und virtuelle Hochgeschwindigkeitskanäle (Warps, Feeds).
// Die Adressstruktur ist universell und erweiterbar – sie ermöglicht
// präzise Zielsteuerung von Protonen-Schub, Protein-Ausführung und
// Master-Control-Operationen.
// Drei Hauptklassen:
// - Legacy (B32) – Kompatibilität zu externen Systemen
// - High-Speed (B64) – Interne BMC-Pfade
// - Systemintern – Boxis, Feed, Warp, Node, Gatter

typedef enum {
    ADRESS_TYP_LEGACY     = 0,      // B32 – Externe RAM-Bereiche (x86-kompatibel)
    ADRESS_TYP_HIGH_SPEED = 1,      // B64 – Direkter interner BMC-Zugriff
    ADRESS_TYP_BOXIS      = 2,      // Direkte Boxis-ID
    ADRESS_TYP_FEED       = 3,      // Feed-ID + Position (z. B. Ring-Offset)
    ADRESS_TYP_WARP       = 4,      // Warp-Kanal-ID + Phase (0–7 bei 8-Takt)
    ADRESS_TYP_NODE       = 5,      // Master-Control-Node (Familienhierarchie)
    ADRESS_TYP_GATTER     = 6       // Physische Gatter-Adresse (Needle-Ziel)
} adress_typ_t;

// =============================================================================
// Universelle Adressstruktur
// =============================================================================

typedef struct mcs_adresse {
    // Typ der Adresse – bestimmt Interpretation der union
    adress_typ_t        typ;

    // Adresswert – je nach Typ unterschiedlich belegt
    union {
        B32             legacy_pfAD;        // Legacy-Adresse (extern)
        B64             high_speed_pfAD;    // High-Speed-Pfad im BMC
        B64             boxis_id;
        B64             feed_id;
        B64             warp_id;
        B64             node_id;
        B64             gatter_adresse;    // Physische Position im BMC
    } wert;

    // Zusatzparameter – für verschachtelte oder zeitliche Adressierung
    B64                 offset;             // z. B. Position in Feed/Ring (Phase 0–7)
    B64                 sub_adresse;        // z. B. Protein in Boxis, Proton in Feed

    // Cache für schnelle Auflösung – optional vom Resolver gefüllt
    union {
        void*           ziel_ptr;           // Generischer Zeiger
        struct mcs_boxis* boxis;
        struct mcs_feed*  feed;
        struct mcs_warp*  warp;
        struct mcs_protein* protein;
        struct mcs_proton* proton;
    } cache;

    // Validität und Metadaten
    B1                  gueltig;            // 1 = Adresse erfolgreich aufgelöst
    B64                 timestamp;          // Zeitpunkt der letzten Auflösung
    B64                 prioritaet;         // Priorität bei konkurrierenden Zugriffen

    // Zukunftssicherheit (z. B. kryptogenetische Prüfung, AI-Routing)
    B64                 reserved[5];

} mcs_adresse_t;

// =============================================================================
// Standard-Adressen
// =============================================================================

#define MCS_ADRESSE_NULL { \
.typ                = ADRESS_TYP_LEGACY, \
.wert               = { .legacy_pfAD = 0 }, \
.offset             = 0, \
.sub_adresse        = 0, \
.cache              = { .ziel_ptr = NULL }, \
.gueltig            = 0, \
.timestamp          = 0, \
.prioritaet         = 0, \
.reserved           = {0} \
}

// Beispiel: Interne High-Speed-Adresse
#define MCS_ADRESSE_HIGH_SPEED(pfad) { \
.typ                = ADRESS_TYP_HIGH_SPEED, \
.wert               = { .high_speed_pfAD = (pfad) }, \
.offset             = 0, \
.sub_adresse        = 0, \
.cache              = { .ziel_ptr = NULL }, \
.gueltig            = 0, \
.timestamp          = 0, \
.prioritaet         = 80, \
.reserved           = {0} \
}

// Beispiel: Warp-Adresse mit 8-Takt-Phase
#define MCS_ADRESSE_WARP_RING(warp_id_val, phase) { \
.typ                = ADRESS_TYP_WARP, \
.wert               = { .warp_id = (warp_id_val) }, \
.offset             = (phase), \
.sub_adresse        = 0, \
.cache              = { .ziel_ptr = NULL }, \
.gueltig            = 0, \
.timestamp          = 0, \
.prioritaet         = 99, \
.reserved           = {0} \
}

// Beispiel: Direkte Boxis-Adresse
#define MCS_ADRESSE_BOXIS(boxis_id_val) { \
.typ                = ADRESS_TYP_BOXIS, \
.wert               = { .boxis_id = (boxis_id_val) }, \
.offset             = 0, \
.sub_adresse        = 0, \
.cache              = { .ziel_ptr = NULL }, \
.gueltig            = 0, \
.timestamp          = 0, \
.prioritaet         = 70, \
.reserved           = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_adresse_ist_gueltig(const mcs_adresse_t* a) {
    return a && a->gueltig == 1;
}

static inline bool mcs_adresse_ist_high_speed(const mcs_adresse_t* a) {
    return a && a->typ == ADRESS_TYP_HIGH_SPEED;
}

static inline bool mcs_adresse_ist_warp(const mcs_adresse_t* a) {
    return a && a->typ == ADRESS_TYP_WARP;
}

static inline bool mcs_adresse_ist_legacy(const mcs_adresse_t* a) {
    return a && a->typ == ADRESS_TYP_LEGACY;
}

static inline bool mcs_adresse_hat_cache(const mcs_adresse_t* a) {
    return a && a->cache.ziel_ptr != NULL;
}

static inline B64 mcs_adresse_get_warp_phase(const mcs_adresse_t* a) {
    return (a && a->typ == ADRESS_TYP_WARP) ? a->offset : 0;
}

// =============================================================================
// Ende Modul 14
// =============================================================================

#endif // MCS_14_ADRESSIERUNG_H
