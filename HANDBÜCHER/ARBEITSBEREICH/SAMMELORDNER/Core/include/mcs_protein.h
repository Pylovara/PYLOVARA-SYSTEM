// include/mcs_protein.h
#ifndef MCS_PROTEIN_H
#define MCS_PROTEIN_H

#include "mcs_action.h"
#include "mcs_proton.h"
#include "mcs_warp.h"
#include "mcs_wahrheiten.h"

// 🔹 Forward-Deklaration als typedef — NICHT struct mcs_protein
typedef struct mcs_protein mcs_protein_t;

struct mcs_protein {
    mcs_action_t* action;
    mcs_proton_t* proton;
    mcs_warp_t*   warp;
    mcs_wahrheit_t wahrheit;
    mcs_protein_t* then_branch;   // ← jetzt vom selben Typ
    mcs_protein_t* else_branch;   // ← jetzt vom selben Typ
};

// Public API
mcs_protein_t* mcs_parse_protein(const char* input);
void mcs_free_protein(mcs_protein_t* p);

#endif
