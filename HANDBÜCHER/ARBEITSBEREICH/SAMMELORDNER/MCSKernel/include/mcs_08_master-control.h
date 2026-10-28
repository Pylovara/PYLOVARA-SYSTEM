// =============================================================================
// include/mcs_08_master-control.h
// Pylovara MCS Kernel – Modul 08: MASTER-CONTROL
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Herzstück des Systems – Familienhierarchie, Magic-Brücke, Needles, Sync-Timer
// =============================================================================

#ifndef MCS_08_MASTER_CONTROL_H
#define MCS_08_MASTER_CONTROL_H

#include "mcs_01_datentypen.h"           // B64, B1
#include "mcs_07_boxis.h"                // WICHTIG: mcs_boxis_t
#include "mcs_12_feeds.h"                // optional, für zukünftige Erweiterung
#include "mcs_13_warps.h"                 // optional, für zukünftige Erweiterung
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// MASTER-CONTROL – Vererbungsprinzip und Hierarchie
// =============================================================================

// Die Hierarchie folgt dem biologischen Familienmodell und ist bidirektional:
// Höhere Ebenen können auf tiefere zugreifen, tiefere auf höhere.
// Dies ermöglicht dynamische Umschaltung, Konfigurationsbrücken und
// direkten Durchschlag (Needles) in physische Logik.

typedef enum {
    MC_LEVEL_KERNEL = 0,        // 0 *.*-kernel – Unabhängige Kernelschicht
    MC_LEVEL_CORE   = 1,        // 1 *.*-core – Großvater (Ausführender Teil)
    MC_LEVEL_MAGIC  = 2,        // 2 *.*-magic – Großmutter (Umschalter, Sync-Timer)
    MC_LEVEL_NODE   = 3,        // 3 *.*-node – Vater (Hauptcontroller)
    MC_LEVEL_NANO   = 4,        // Mutter – Substrukturmanager
    MC_LEVEL_MICRO  = 5,        // Tochter – Mikrosteuerung
    MC_LEVEL_CACHE  = 6,        // Cache – Temp-Daten
    MC_LEVEL_NEEDLES = 7        // Needles – Hardwarebindung (Durchschlag)
} mc_hierarchie_level_t;

// =============================================================================
// Sync-Timer (⊕) – Herzschlag des Systems
// =============================================================================

// Der Sync-Timer steuert alle zeitlichen Abläufe (8-Takt, Zyklen, Phase).
// Er kann dynamisch angepasst werden (z. B. durch AI).

typedef struct mcs_sync_timer {
    B64     global_tick;        // Systemweiter Zähler
    B64     takt_rate;          // Basis-Takt (z. B. 8 für Lichtkreise)
    B64     current_phase;      // Aktuelle Phase (0–7 bei 8-Takt)
    B1      paused;             // 1 = Timer angehalten
    B64     subscribers;        // Anzahl angemeldeter Nodes/Magic/Boxis
    B64     reserved[8];        // Zukunftssicherheit
} mcs_sync_timer_t;

// =============================================================================
// Needle – Direkte physische Hardware-Ankopplung
// =============================================================================

// Needles sind die "Einstichpunkte" – direkter Durchschlag in physische Gatter.
// Sie ermöglichen Spannung, Impact und Signaltransfer ohne RAM-Zwischenspeicher.

typedef struct mcs_needle {
    B64                 needle_id;
    mc_hierarchie_level_t level;        // Immer MC_LEVEL_NEEDLES
    B64                 gatter_adresse; // Physische Gatter-Position im BMC
    B64                 spannung_current; // Aktuelle Spannung / Lichtintensität
    B64                 impact_schub;   // Letzter Proton-Impact
    B1                  aktiv;          // 1 = Needle eingestochen und aktiv
    B64                 zugriffs_node;  // Welcher Node kontrolliert die Needle
    struct mcs_needle*  next_needle;
    struct mcs_needle*  prev_needle;
    B64                 reserved[6];
} mcs_needle_t;

// =============================================================================
// Node – Hauptcontroller (Vater + Familie)
// =============================================================================

typedef struct mcs_node {
    B64                 node_id;
    mc_hierarchie_level_t level;        // MC_LEVEL_NODE oder Familie
    const char*         name;           // z. B. "Hauptcontroller"

    // Vererbung und Familie
    struct mcs_node*    parent_core;    // Großvater
    struct mcs_magic*   magic_bruecke;  // Großmutter (Sync-Timer)
    struct mcs_node*    nano_mutter;    // Substrukturmanager
    struct mcs_node*    micro_tochter;  // Mikrosteuerung
    mcs_boxis_t*        cache;          // Temp-Daten
    mcs_needle_t*       needles_kopf;   // Hardware-Ankopplungen

    // Globale Kontrolle
    mcs_sync_timer_t*   sync_timer;     // Zeiger auf globalen Timer
    B64                 prioritaet_global;

    // Zustand
    B1                  aktiv;
    B64                 letzte_aktion_zeit;
    B64                 zugriffe_seit_start;

    // Verkettung aller Nodes
    struct mcs_node*    next_node;
    struct mcs_node*    prev_node;

    B64                 reserved[10];
} mcs_node_t;

// =============================================================================
// Magic – Dynamische Umschalt- und Konfigurationsbrücke
// =============================================================================

typedef struct mcs_magic {
    B64                 magic_id;
    mc_hierarchie_level_t level;        // MC_LEVEL_MAGIC
    mcs_sync_timer_t    sync_timer;     // Integrierter ⊕ Sync-Timer
    B64                 umschalt_knoten_anzahl;
    mcs_node_t**        verbundene_nodes; // Dynamisch umschaltbare Vater-Nodes
    B1                  ai_gesteuert;   // 1 = AI darf Timer/Priorität anpassen
    B64                 dynamische_takt_rate; // Kann von 8 auf andere Werte wechseln
    struct mcs_magic*   next_magic;
    struct mcs_magic*   prev_magic;
    B64                 reserved[8];
} mcs_magic_t;

// =============================================================================
// Globaler Master-Control – Singleton
// =============================================================================

typedef struct mcs_master_control {
    // Hierarchie-Wurzeln
    mcs_node_t*         kernel_root;
    mcs_node_t*         core_grossvater;
    mcs_magic_t*        magic_grossmutter;

    // Globale Register und Listen
    mcs_node_t*         alle_nodes_kopf;
    mcs_needle_t*       alle_needles_kopf;
    mcs_boxis_t*        alle_boxis_kopf;
    B64                 system_start_zeit;
    B64                 global_tick_count;
    B1                  running;

    B64                 reserved[16];
} mcs_master_control_t;

// Globale Instanz
extern mcs_master_control_t MC_Global;

// =============================================================================
// Funktionsdeklarationen
// =============================================================================

void mcs_master_control_initialisieren(void);
void mcs_sync_tick(void);
void mcs_needle_einstich(mcs_needle_t* needle);
void mcs_magic_umschalten(mcs_node_t* neuer_vater);

#endif // MCS_08_MASTER_CONTROL_H
