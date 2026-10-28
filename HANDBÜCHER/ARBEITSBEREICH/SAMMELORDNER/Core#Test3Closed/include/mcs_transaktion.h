// include/mcs_transaktion.h — v3.0 + Operators (zyklusfrei!)
#ifndef MCS_TRANS_H
#define MCS_TRANS_H

#include "mcs_token.h"
#include "mcs_protein.h"

// 🔹 Forward-Deklaration statt Include (bricht Zyklus!)
struct mcs_operator;
typedef struct mcs_operator mcs_operator_t;

typedef struct {
    mcs_token_t trans_start;
    mcs_token_t trans_end;
    mcs_wahrheit_t wahrheit;
    mcs_protein_t* condition;

    // 🔹 Operatoren (kernel 09) — nur Zeiger, keine Definition nötig
    mcs_operator_t** operators;
    int operator_count;
    mcs_protein_t* error_handler; // für ^

    mcs_protein_t** proteine;
    int protein_count;
    int is_valid;
} mcs_transaktion_t;

// Public API
mcs_transaktion_t* mcs_parse_transaktion(const char* input);
void mcs_free_transaktion(mcs_transaktion_t* t);

#endif // MCS_TRANS_H


