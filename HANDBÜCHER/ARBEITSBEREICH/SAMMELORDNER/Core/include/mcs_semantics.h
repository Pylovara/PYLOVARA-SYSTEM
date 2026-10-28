// include/mcs_sematics.h - Wahrheitstabelle & Bedeutungslogik
// include/mcs_semantics.h — v3.0 Semantik-Kern
#ifndef MCS_SEMANTICS_H
#define MCS_SEMANTICS_H

#include "mcs_token.h"
#include "mcs_protein.h"

// Logische Wahrheitswerte (nicht BOOL — explizit klar)
typedef enum {
    MCS_FALSE = 0,
    MCS_TRUE = 1,
    MCS_ERROR = -1   // Fehler bei Auswertung (z.B. SIGKILL)
} mcs_truth_t;

// Semantische Bedeutung jeder Wahrheit
typedef enum {
    SEMANTIC_IF,          // ¶ — führe aus, WENN Bedingung TRUE
    SEMANTIC_ELSE,        // ¶¶ — führe aus, WENN vorherige Bedingung FALSE
    SEMANTIC_WHEN_NOT,    // ° — führe aus, WENN Bedingung FALSE
    SEMANTIC_MUST,        // ! — Sofort, unbedingt (keine Bedingung, Timeout-gesteuert)
    SEMANTIC_ON_ERROR,    // … — Nur bei vorherigem Fehler
    SEMANTIC_PARALLEL,    // ¶= — Fork & Exec
    SEMANTIC_PARALLEL_TRANSPORT, // ;; — Datenfluss ohne Blockieren
    SEMANTIC_NONE
} mcs_semantic_op_t;

// Öffentliche API
mcs_semantic_op_t mcs_wahrheit_to_semantic(mcs_wahrheit_t w);
mcs_truth_t mcs_evaluate_protein(mcs_protein_t* p);  // führt aus & gibt MCS_TRUE/FALSE zurück
mcs_truth_t mcs_evaluate_condition(mcs_protein_t* cond); // Nur Bedingung prüfen, nicht ausführen

#endif
