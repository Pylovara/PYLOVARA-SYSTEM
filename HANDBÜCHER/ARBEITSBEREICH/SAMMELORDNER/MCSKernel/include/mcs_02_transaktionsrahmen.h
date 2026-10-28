// =============================================================================
// include/mcs_02_transaktionsrahmen.h
// Pylovara MCS Kernel – Modul 02: Transaktionsrahmen
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: VALIDE
// =============================================================================

#ifndef MCS_02_TRANSAKTIONSRAHMEN_H
#define MCS_02_TRANSAKTIONSRAHMEN_H

#include "mcs_01_datentypen.h"     // B8, B64
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// Symbolische Deklarationen des Transaktionsrahmens
// =============================================================================

// Transaktionsrahmen-Marker gemäß MCS-Syntax
#define TRANS_START       "¢!"    // Cent-Zeichen Ausrufezeichen → Transaktionsanfang
#define TRANS_END         "!¢"    // Ausrufezeichen Cent-Zeichen → Transaktionsende

// Lesbare Aliase für Debugging und Logging
#define TRANS_BEGIN       TRANS_START
#define TRANS_CLOSE       TRANS_END

// =============================================================================
// Transaktionsklassen
// Bestimmen die Richtung und Funktion des Rahmens innerhalb der MCS-Runtime
// =============================================================================

typedef enum {
    MCS_RUNTIME_ANFANGSBEDIENUNG = 0,   // ¢! → Eingabe / Prozessstart
    MCS_RUNTIME_ENDBEDIENUNG     = 1    // !¢ → Ausgabe / Prozessende
} mcs_transaktionsklasse_t;

// =============================================================================
// Grundstruktur eines Transaktionsrahmens
// Jede gültige MCS-Transaktion ist in einen solchen Rahmen eingebettet
// =============================================================================

typedef struct mcs_transaktionsrahmen {
    // Symbolische Marker (für Parser und Debugging)
    // Können später durch binäre Kennung ersetzt werden
    const char* start_marker;           // Zeiger auf "¢!"
    const char* end_marker;             // Zeiger auf "!¢"

    // Klassentyp der Transaktion
    mcs_transaktionsklasse_t klasse;

    // Metadaten (werden vom Parser/Executor gefüllt)
    B64 timestamp;                      // Zeitpunkt des Rahmenstarts
    B64 protein_count;                  // Anzahl enthaltener Proteine
    B64 reserved[4];                    // Reserviert für zukünftige Erweiterungen
} mcs_transaktionsrahmen_t;

// =============================================================================
// Standard-Initialisierungen für Transaktionsrahmen
// =============================================================================

#define MCS_TRANS_RAHMEN_START { \
.start_marker  = TRANS_START, \
.end_marker    = TRANS_END, \
.klasse        = MCS_RUNTIME_ANFANGSBEDIENUNG, \
.timestamp     = 0, \
.protein_count = 0, \
.reserved      = {0} \
}

#define MCS_TRANS_RAHMEN_ENDE { \
.start_marker  = TRANS_START, \
.end_marker    = TRANS_END, \
.klasse        = MCS_RUNTIME_ENDBEDIENUNG, \
.timestamp     = 0, \
.protein_count = 0, \
.reserved      = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// Ermöglichen schnelle Prüfung des Transaktionsstatus
// =============================================================================

static inline bool mcs_transaktion_ist_start(const mcs_transaktionsrahmen_t* rahmen) {
    return rahmen && rahmen->klasse == MCS_RUNTIME_ANFANGSBEDIENUNG;
}

static inline bool mcs_transaktion_ist_ende(const mcs_transaktionsrahmen_t* rahmen) {
    return rahmen && rahmen->klasse == MCS_RUNTIME_ENDBEDIENUNG;
}

// =============================================================================
// Ende Modul 02
// =============================================================================

#endif // MCS_02_TRANSAKTIONSRAHMEN_H
