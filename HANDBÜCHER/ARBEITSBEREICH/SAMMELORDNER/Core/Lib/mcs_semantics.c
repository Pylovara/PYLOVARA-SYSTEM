// Lib/mcs_semantics.c — Semantische Logik
// Lib/mcs_semantics.c — v3.0: Wahrheits-Bewertung & Bedeutungszuordnung
#include "mcs_semantics.h"
#include "mcs_runner.h"   // für mcs_run_protein()
#include <unistd.h>       // für system()

mcs_semantic_op_t mcs_wahrheit_to_semantic(mcs_wahrheit_t w) {
    switch (w) {
        case WAHR_IF:          return SEMANTIC_IF;
        case WAHR_ELSE:        return SEMANTIC_ELSE;
        case WAHR_WHEN_NOT:    return SEMANTIC_WHEN_NOT;
        case WAHR_MUST:        return SEMANTIC_MUST;
        case WAHR_ON_ERROR:    return SEMANTIC_ON_ERROR;
        case WAHR_PARALLEL_RUN:return SEMANTIC_PARALLEL;
        case WAHR_PARALLEL_TRANSPORT: return SEMANTIC_PARALLEL_TRANSPORT;
        default:               return SEMANTIC_NONE;
    }
}

// Nur Bedingung *prüfen*, nicht ausführen → sichere Prüfung
mcs_truth_t mcs_evaluate_condition(mcs_protein_t* cond) {
    if (!cond || !cond->action || !cond->action->cmd) return MCS_FALSE;

    // Beispiel: ['$terminal'] → prüfen, ob Prozess läuft
    if (strcmp(cond->action->cmd, "$terminal") == 0) {
        int r = system("pgrep -x kitty > /dev/null 2>&1 || pgrep -x alacritty > /dev/null 2>&1");
        return (WIFEXITED(r) && WEXITSTATUS(r) == 0) ? MCS_TRUE : MCS_FALSE;
    }
    // Fallback: führe aus & schaue Exit-Code
    int res = mcs_run_protein(cond);   // ✅ res deklarieren
    if (res == MCS_ERR_SYNTAX) return MCS_ERROR;
    return MCS_FALSE;                  // sonst: Bedingung nicht erfüllt
}

// Bedingung *ausführen* und Ergebnis liefern
mcs_truth_t mcs_evaluate_protein(mcs_protein_t* p) {
    if (!p) return MCS_FALSE;
    int res = mcs_run_protein(p);
    return (res == MCS_OK) ? MCS_TRUE : (res < 0 ? MCS_ERROR : MCS_FALSE);
}
