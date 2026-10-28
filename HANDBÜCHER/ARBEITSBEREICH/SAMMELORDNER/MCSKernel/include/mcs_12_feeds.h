// =============================================================================
// include/mcs_12_feeds.h
// Pylovara MCS Kernel – Modul 12: Feeds
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Datenströme und verkettete Transportkanäle
// =============================================================================

#ifndef MCS_12_FEEDS_H
#define MCS_12_FEEDS_H

#include "mcs_01_datentypen.h"           // B64, B1
#include "mcs_03_protein.h"              // mcs_protein_t
#include "mcs_04_proton.h"               // mcs_proton_t
#include "mcs_07_boxis.h"                // mcs_boxis_t – Feeds können gepuffert werden
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Feeds – Grunddefinition
// =============================================================================

// Feeds sind geordnete, verkettete Datenströme im Pylovara-System.
// Sie transportieren Proteine und Protonen von einer Boxis zur nächsten,
// von einem Node zum anderen oder als zyklische Struktur (Ring).
// Feeds sind der **normale Transportweg** – im Gegensatz zu Warps (Hochgeschwindigkeit).
// Sie unterstützen verschiedene Betriebsmodi (Queue, Stack, Ring, Stream, Cache)
// und sind vollständig durch Master-Control steuerbar.
// Der Ring-Feed ist speziell für photonische 8-Takt-Lichtkreise optimiert.

typedef enum {
    FEED_QUEUE = 0,                 // FIFO – Standard-Datenstrom
    FEED_STACK = 1,                 // LIFO – für temporäre Rückwärtsverarbeitung
    FEED_RING = 2,                  // Zyklisch – für 8-Takt-Lichtkreise und Resonatoren
    FEED_STREAM = 3,                // Kontinuierlicher Fluss ohne definites Ende
    FEED_CACHE = 4                  // Zwischengespeicherter Feed mit Reinigungsfunktion
} feed_typ_t;

// Betriebszustand eines Feeds
typedef enum {
    FEED_LEER = 0,                  // Kein Inhalt vorhanden
    FEED_AKTIV = 1,                 // Daten fließen aktiv
    FEED_PAUSIERT = 2,              // Durch Master-Control angehalten
    FEED_GESPERRT = 3,              // Durch Trennbefehl blockiert
    FEED_FEHLER = 4                 // Interner Fehler (z. B. Überlauf)
} feed_status_t;

// =============================================================================
// Hauptstruktur eines Feeds
// =============================================================================

typedef struct mcs_feed {
    // Identifikation und Klassifikation
    B64                 feed_id;            // Eindeutige System-ID
    feed_typ_t          typ;

    // Aktueller Betriebszustand
    feed_status_t       status;

    // Inhalt – doppelt verkettet für effiziente Navigation
    struct mcs_protein* protein_kopf;       // Erstes Protein im Feed
    struct mcs_protein* protein_schwanz;    // Letztes Protein (schnelles Anhängen)
    struct mcs_proton*  proton_kopf;        // Erstes Proton
    struct mcs_proton*  proton_schwanz;     // Letztes Proton

    B64                 element_anzahl;     // Gesamtanzahl (Protein + Proton)
    B64                 max_kapazitaet;     // Sicherheitsobergrenze (Überlaufschutz)

    // Flusssteuerung – Sicherheitsventile
    B1                  eingang_offen;      // 1 = Neue Elemente dürfen eintreten
    B1                  ausgang_offen;      // 1 = Elemente dürfen austreten
    B64                 geschwindigkeit;    // Elemente pro Sync-Timer-Zyklus (normal: 1)

    // Ring-spezifische Parameter (nur bei FEED_RING relevant)
    B64                 ring_position;      // Aktuelle Leseposition (0–7 bei 8-Takt)
    B64                 ring_groesse;       // Fixe Größe des Rings (z. B. 8)

    // Pufferung in sicherer Umgebung
    struct mcs_boxis*   zugeordnete_boxis;  // Optional: Feed läuft durch Buffer-Boxis

    // Scheduling und Master-Control
    B64                 sync_timer;         // Verknüpfung mit Magic-Timer
    B64                 priority;           // Priorität beim Scheduling
    B64                 zugriffs_node;      // Welcher Master-Control-Node steuert den Feed

    // Metadaten für Diagnose und Nachvollziehbarkeit
    B64                 erstellungs_zeit;
    B64                 letzte_aktivitaet;
    B64                 transfer_counter;   // Wie viele Elemente durchgelaufen sind

    // Zukunftssicherheit (z. B. Filter, Kompression, AI-Steuerung)
    B64                 reserved[6];

    // Verkettung aller Feeds – für globale Master-Control-Übersicht
    struct mcs_feed*    next_feed;
    struct mcs_feed*    prev_feed;

} mcs_feed_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_FEED_NULL { \
.feed_id             = 0, \
.typ                 = FEED_QUEUE, \
.status              = FEED_LEER, \
.protein_kopf        = NULL, \
.protein_schwanz     = NULL, \
.proton_kopf         = NULL, \
.proton_schwanz      = NULL, \
.element_anzahl      = 0, \
.max_kapazitaet      = 10000, \
.eingang_offen       = 1, \
.ausgang_offen       = 1, \
.geschwindigkeit     = 1, \
.ring_position       = 0, \
.ring_groesse        = 0, \
.zugeordnete_boxis   = NULL, \
.sync_timer          = 0, \
.priority            = 50, \
.zugriffs_node       = 0, \
.erstellungs_zeit    = 0, \
.letzte_aktivitaet   = 0, \
.transfer_counter    = 0, \
.reserved            = {0}, \
.next_feed           = NULL, \
.prev_feed           = NULL \
}

// Beispiel: 8-Takt Ring-Feed für Lichtkreise
#define MCS_FEED_RING_8_TAKT(id) { \
.feed_id             = (id), \
.typ                 = FEED_RING, \
.status              = FEED_LEER, \
.protein_kopf        = NULL, \
.protein_schwanz     = NULL, \
.proton_kopf         = NULL, \
.proton_schwanz      = NULL, \
.element_anzahl      = 0, \
.max_kapazitaet      = 8, \
.eingang_offen       = 1, \
.ausgang_offen       = 1, \
.geschwindigkeit     = 1, \
.ring_position       = 0, \
.ring_groesse        = 8, \
.zugeordnete_boxis   = NULL, \
.sync_timer          = 0, \
.priority            = 90, \
.zugriffs_node       = 0, \
.erstellungs_zeit    = 0, \
.letzte_aktivitaet   = 0, \
.transfer_counter    = 0, \
.reserved            = {0}, \
.next_feed           = NULL, \
.prev_feed           = NULL \
}

// Beispiel: High-Priority Cache-Feed
#define MCS_FEED_CACHE_HIGH_PRIO { \
.feed_id             = 0, /* vom System vergeben */ \
.typ                 = FEED_CACHE, \
.status              = FEED_LEER, \
.protein_kopf        = NULL, \
.protein_schwanz     = NULL, \
.proton_kopf         = NULL, \
.proton_schwanz      = NULL, \
.element_anzahl      = 0, \
.max_kapazitaet      = 1000, \
.eingang_offen       = 1, \
.ausgang_offen       = 1, \
.geschwindigkeit     = 10, \
.ring_position       = 0, \
.ring_groesse        = 0, \
.zugeordnete_boxis   = NULL, \
.sync_timer          = 0, \
.priority            = 95, \
.zugriffs_node       = 0, \
.erstellungs_zeit    = 0, \
.letzte_aktivitaet   = 0, \
.transfer_counter    = 0, \
.reserved            = {0}, \
.next_feed           = NULL, \
.prev_feed           = NULL \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_feed_ist_leer(const mcs_feed_t* f) {
    return f && (f->status == FEED_LEER || f->element_anzahl == 0);
}

static inline bool mcs_feed_ist_aktiv(const mcs_feed_t* f) {
    return f && f->status == FEED_AKTIV;
}

static inline bool mcs_feed_ist_ring(const mcs_feed_t* f) {
    return f && f->typ == FEED_RING;
}

static inline bool mcs_feed_eingang_erlaubt(const mcs_feed_t* f) {
    return f && f->eingang_offen == 1 && f->status != FEED_GESPERRT;
}

static inline bool mcs_feed_ausgang_erlaubt(const mcs_feed_t* f) {
    return f && f->ausgang_offen == 1 && f->status != FEED_GESPERRT;
}

static inline bool mcs_feed_ist_voller_ring(const mcs_feed_t* f) {
    return mcs_feed_ist_ring(f) && f->element_anzahl >= f->ring_groesse;
}

// =============================================================================
// Ende Modul 12
// =============================================================================

#endif // MCS_12_FEEDS_H
