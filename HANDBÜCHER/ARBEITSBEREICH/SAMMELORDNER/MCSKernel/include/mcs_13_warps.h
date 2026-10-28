// =============================================================================
// include/mcs_13_warp.h
// Pylovara MCS Kernel – Modul 13: Warps
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Hochgeschwindigkeits-Transportkanäle (photonisch inspiriert)
// =============================================================================

#ifndef MCS_13_WARPS_H
#define MCS_13_WARPS_H

#include "mcs_01_datentypen.h"           // B64
#include "mcs_03_protein.h"              // mcs_protein_t – optional Transport
#include "mcs_04_proton.h"               // mcs_proton_t – primärer Transport
#include "mcs_07_boxis.h"                // mcs_boxis_t – Warp-Gates
#include "mcs_12_feeds.h"                // mcs_feed_t – Warps als schnelle Alternative
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Warps – Grunddefinition
// =============================================================================

// Warps sind die Hochgeschwindigkeits-Transportkanäle des Pylovara-Systems.
// Sie ermöglichen nahezu latenzfreien Transfer von Protonen (und optional Proteinen)
// zwischen Boxis, Nodes oder über große Distanzen.
// Stark inspiriert von photonischen Lichtkreisen: Ein Proton wird als Lichtimpuls
// durch einen dedizierten Kanal gesendet – mit minimaler Verzögerung und
// maximaler Bandbreite.
// Warps sind der "Lichtweg" – im Gegensatz zu normalen Feeds (elektronisch).
// Der Ring-Warp ist speziell für 8-Takt-Resonatoren optimiert.

typedef enum {
    WARP_PHOTONISCH_RING = 0,       // Zyklischer Lichtkreis (8-Takt, Resonator)
    WARP_PHOTONISCH_LINE = 1,       // Gerader Lichtleiter (Punkt-zu-Punkt)
    WARP_HYBRID = 2,                // Photonisch + elektronisch (Fallback)
    WARP_QUANTUM_SIM = 3,           // Platzhalter für zukünftige Quantenkanäle
    WARP_VIRTUAL = 4                // Software-Direkttransfer (Fallback)
} warp_typ_t;

// Betriebszustand eines Warp-Kanals
typedef enum {
    WARP_INAKTIV = 0,               // Kanal ausgeschaltet
    WARP_BEREIT = 1,                // Initialisiert, wartet auf Proton
    WARP_AKTIV = 2,                 // Transfer aktiv (Proton im Kanal)
    WARP_OVERLOAD = 3,              // Nichtlineare Effekte durch zu viele Protonen
    WARP_FEHLER = 4                 // Technischer Defekt (z. B. Resonanzdrift)
} warp_status_t;

// =============================================================================
// Hauptstruktur eines Warp-Kanals
// =============================================================================

typedef struct mcs_warp {
    // Identifikation und Klassifikation
    B64                 warp_id;            // Eindeutige Kanal-ID
    warp_typ_t          typ;

    // Aktueller Betriebszustand
    warp_status_t       status;

    // Quelle und Ziel – immer über Warp-Gate-Boxis
    struct mcs_boxis*   quelle_boxis;       // Eingangs-Warp-Gate
    struct mcs_boxis*   ziel_boxis;         // Ausgangs-Warp-Gate
    B64                 quelle_node;        // Ursprungs-Node
    B64                 ziel_node;          // Ziel-Node

    // Aktuell transportiertes Element
    struct mcs_proton*  aktives_proton;     // Primär: ein Proton pro Zyklus
    struct mcs_protein* aktives_protein;    // Optional bei HYBRID
    B64                 aktuelle_position;  // Phase im Ring (0–7 bei 8-Takt)

    // Photonische Kanalparameter
    B64                 bandbreite;         // Max. Protonen pro Zyklus (WDM)
    B64                 latenz_zyklen;      // Typische Verzögerung (ideal 8 bei Ring)
    B64                 wellenlaenge_id;    // Resonanz-Wellenlänge (z. B. 1550 nm)
    B64                 ring_phase_shift;   // Phasenversatz für Abtastung

    // Überlastschutz
    B64                 max_simultan_protonen;
    B64                 overload_counter;

    // Scheduling und Master-Control
    B64                 sync_timer;         // Strenge Synchronisation mit Magic-Timer
    B64                 priority;           // Priorität bei Kanalvergabe
    B64                 zugriffs_node;      // Kontrollierender Node

    // Metadaten für Diagnose und Leistung
    B64                 erstellungs_zeit;
    B64                 letzte_nutzung;
    B64                 transfer_counter;   // Erfolgreich transportierte Protonen
    B64                 fehler_counter;

    // Zukunftssicherheit (z. B. Polarisation, Kerr-Effekt, Quantenmetriken)
    B64                 reserved[6];

    // Verkettung aller Warp-Kanäle – für globale Master-Control-Übersicht
    struct mcs_warp*    next_warp;
    struct mcs_warp*    prev_warp;

} mcs_warp_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_WARP_NULL { \
.warp_id                = 0, \
.typ                    = WARP_PHOTONISCH_RING, \
.status                 = WARP_INAKTIV, \
.quelle_boxis           = NULL, \
.ziel_boxis             = NULL, \
.quelle_node            = 0, \
.ziel_node              = 0, \
.aktives_proton         = NULL, \
.aktives_protein        = NULL, \
.aktuelle_position      = 0, \
.bandbreite             = 1, \
.latenz_zyklen          = 8, \
.wellenlaenge_id        = 0, \
.ring_phase_shift       = 1, \
.max_simultan_protonen  = 1, \
.overload_counter       = 0, \
.sync_timer             = 0, \
.priority               = 95, \
.zugriffs_node          = 0, \
.erstellungs_zeit       = 0, \
.letzte_nutzung         = 0, \
.transfer_counter       = 0, \
.fehler_counter         = 0, \
.reserved               = {0}, \
.next_warp              = NULL, \
.prev_warp              = NULL \
}

// Speziell: 8-Takt photonischer Ring-Warp
#define MCS_WARP_PHOTONISCH_8_TAKT_RING(id, quelle_box, ziel_box) { \
.warp_id                = (id), \
.typ                    = WARP_PHOTONISCH_RING, \
.status                 = WARP_BEREIT, \
.quelle_boxis           = (quelle_box), \
.ziel_boxis             = (ziel_box), \
.quelle_node            = 0, \
.ziel_node              = 0, \
.aktives_proton         = NULL, \
.aktives_protein        = NULL, \
.aktuelle_position      = 0, \
.bandbreite             = 1, \
.latenz_zyklen          = 8, \
.wellenlaenge_id        = 1550, /* Beispiel: 1550 nm C-Band */ \
.ring_phase_shift       = 1, \
.max_simultan_protonen  = 1, \
.overload_counter       = 0, \
.sync_timer             = 0, \
.priority               = 99, \
.zugriffs_node          = 0, \
.erstellungs_zeit       = 0, \
.letzte_nutzung         = 0, \
.transfer_counter       = 0, \
.fehler_counter         = 0, \
.reserved               = {0}, \
.next_warp              = NULL, \
.prev_warp              = NULL \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_warp_ist_bereit(const mcs_warp_t* w) {
    return w && w->status == WARP_BEREIT;
}

static inline bool mcs_warp_ist_aktiv(const mcs_warp_t* w) {
    return w && w->status == WARP_AKTIV;
}

static inline bool mcs_warp_ist_photonisch_ring(const mcs_warp_t* w) {
    return w && w->typ == WARP_PHOTONISCH_RING;
}

static inline bool mcs_warp_hat_proton(const mcs_warp_t* w) {
    return w && w->aktives_proton != NULL;
}

static inline bool mcs_warp_ist_overloaded(const mcs_warp_t* w) {
    return w && w->status == WARP_OVERLOAD;
}

// =============================================================================
// Ende Modul 13
// =============================================================================

#endif // MCS_13_WARPS_H
