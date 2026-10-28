// =============================================================================
// include/mcs_11_argumente.h
// Pylovara MCS Kernel – Modul 11: Argumente
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Logische Ketten und Beweisführung
// =============================================================================

#ifndef MCS_11_ARGUMENTE_H
#define MCS_11_ARGUMENTE_H

#include "mcs_01_datentypen.h"           // B64
#include "mcs_06_aktion.h"               // mcs_aktion_t – Argument löst Aktion aus
#include "mcs_09_wahrheiten.h"           // mcs_wahrheit_t – Bausteine des Arguments
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Argumente – Grunddefinition
// =============================================================================

// Argumente sind geordnete Ketten von Wahrheiten, die eine logische Aussage,
// einen Beweis oder eine Entscheidungsgrundlage bilden.
// Sie sind der zentrale Mechanismus der projektiven Rechenlogik und verbinden
// Eingaben (Sentiatoren → Wahrheiten) mit Ausgaben (Aktionen).
// Ein Argument ist immer gerichtet: Prämissen (Voraussetzungen) → Konklusion.
// Der Status wird durch Master-Control geprüft – bei GÜLTIG wird die
// auslösende Aktion gestartet.
// Argumente ermöglichen selbstoptimierende Prozesse und regelbasierte Steuerung.

typedef enum {
    ARGUMENT_BEWEGRÜNDUNG = 0,      // Begründung für eine Aktion (häufigster Fall)
    ARGUMENT_ENSCHEIDUNG  = 1,      // Direkte Master-Control-Entscheidung
    ARGUMENT_REGEL        = 2,      // Fest definierte, immutable Regel
    ARGUMENT_HYPOTHESE    = 3,      // Vorläufige Annahme – kann widerlegt werden
    ARGUMENT_KONFLIKT     = 4       // Widersprüchliche Wahrheiten (Fehlerbehandlung)
} argument_typ_t;

// Status eines Arguments – Ergebnis der logischen Prüfung
typedef enum {
    ARGUMENT_UNGEPRÜFT   = 0,       // Noch nicht validiert
    ARGUMENT_GÜLTIG      = 1,       // Alle Prämissen wahr → Konklusion wahr
    ARGUMENT_UNGÜLTIG    = 2,       // Mindestens eine Prämisse falsch
    ARGUMENT_OFFEN       = 3,       // Enthält UNBESTIMMT-Wahrheiten
    ARGUMENT_WIDERSPRUCH = 4        // Interner logischer Konflikt erkannt
} argument_status_t;

// =============================================================================
// Hauptstruktur eines Arguments
// =============================================================================

typedef struct mcs_argument {
    // Identifikation und Klassifikation
    B64                 argument_id;        // Eindeutige System-ID
    argument_typ_t      typ;

    // Aktueller Prüfstatus
    argument_status_t   status;

    // Prämissen – Eingabe-Wahrheiten (verkettet)
    struct mcs_wahrheit* praemissen_kopf;   // Erste Wahrheit in der Kette
    B64                 praemissen_anzahl;

    // Konklusion – Ergebnis-Wahrheiten (verkettet)
    struct mcs_wahrheit* konklusion_kopf;   // Ergebnis der logischen Ableitung
    B64                 konklusion_anzahl;

    // Auslösende Aktion bei GÜLTIG
    struct mcs_aktion*  ausloesende_aktion; // Wird bei positiver Prüfung gestartet

    // Scheduling und Timing
    B64                 priority;           // Dringlichkeit der Prüfung
    B64                 sync_timer;         // Verknüpfung mit Magic-Timer (periodische Prüfung)
    B1                  einmalig;           // 1 = Nach Auslösung deaktivieren

    // Herkunft und zeitliche Gültigkeit
    B64                 ersteller_node;     // Welcher Master-Control-Node hat erzeugt
    B64                 gueltig_ab;         // Zeitliche Aktivierung
    B64                 gueltig_bis;        // Zeitliche Deaktivierung (0xFFFFFFFFFFFFFFFF = unendlich)

    // Sicherheit und Integrität
    B1                  immutable;          // 1 = Regel – darf nicht verändert werden
    B64                 signatur_hash;      // Kryptogenetische Prüfsumme über die Kette

    // Verkettung aller Argumente – für globale Master-Control-Übersicht
    struct mcs_argument* next_argument;
    struct mcs_argument* prev_argument;

    // Zukunftssicherheit (z. B. AI-Gewichtung, Lernfaktor)
    B64                 reserved[6];

} mcs_argument_t;

// =============================================================================
// Standard-Initialisierung
// =============================================================================

#define MCS_ARGUMENT_NULL { \
.argument_id         = 0, \
.typ                 = ARGUMENT_BEWEGRÜNDUNG, \
.status              = ARGUMENT_UNGEPRÜFT, \
.praemissen_kopf     = NULL, \
.praemissen_anzahl   = 0, \
.konklusion_kopf     = NULL, \
.konklusion_anzahl   = 0, \
.ausloesende_aktion  = NULL, \
.priority            = 50, \
.sync_timer          = 0, \
.einmalig            = 0, \
.ersteller_node      = 0, \
.gueltig_ab          = 0, \
.gueltig_bis         = 0xFFFFFFFFFFFFFFFF, \
.immutable           = 0, \
.signatur_hash       = 0, \
.next_argument       = NULL, \
.prev_argument       = NULL, \
.reserved            = {0} \
}

// Beispiel: Immutable Regel
#define MCS_ARGUMENT_REGEL(id, aktion_ptr) { \
.argument_id         = (id), \
.typ                 = ARGUMENT_REGEL, \
.status              = ARGUMENT_UNGEPRÜFT, \
.praemissen_kopf     = NULL, /* wird zur Laufzeit gefüllt */ \
.praemissen_anzahl   = 0, \
.konklusion_kopf     = NULL, \
.konklusion_anzahl   = 0, \
.ausloesende_aktion  = (aktion_ptr), \
.priority            = 90, \
.sync_timer          = 0, \
.einmalig            = 0, \
.ersteller_node      = 0, \
.gueltig_ab          = 0, \
.gueltig_bis         = 0xFFFFFFFFFFFFFFFF, \
.immutable           = 1, \
.signatur_hash       = 0, \
.next_argument       = NULL, \
.prev_argument       = NULL, \
.reserved            = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_argument_ist_gueltig(const mcs_argument_t* a) {
    return a && a->status == ARGUMENT_GÜLTIG;
}

static inline bool mcs_argument_ist_offen(const mcs_argument_t* a) {
    return a && a->status == ARGUMENT_OFFEN;
}

static inline bool mcs_argument_hat_aktion(const mcs_argument_t* a) {
    return a && a->ausloesende_aktion != NULL;
}

static inline bool mcs_argument_ist_immutable(const mcs_argument_t* a) {
    return a && a->immutable == 1;
}

static inline bool mcs_argument_hat_widerspruch(const mcs_argument_t* a) {
    return a && a->status == ARGUMENT_WIDERSPRUCH;
}

static inline bool mcs_argument_ist_einmalig(const mcs_argument_t* a) {
    return a && a->einmalig == 1;
}

// =============================================================================
// Ende Modul 11
// =============================================================================

#endif // MCS_11_ARGUMENTE_H
