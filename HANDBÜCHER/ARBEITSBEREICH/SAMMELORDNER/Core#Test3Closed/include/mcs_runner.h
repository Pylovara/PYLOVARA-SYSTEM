// include/mcs_runner.h — v3.0 Runner-API
#ifndef MCS_RUNNER_H
#define MCS_RUNNER_H

#include "mcs_transaktion.h"

// 🔹 Forward-Deklaration statt Include (bricht Zyklus!)
struct mcs_operator;
typedef struct mcs_operator mcs_operator_t;

// 🔹 Fehlercodes (öffentliche API — jetzt hier zentral)
#define MCS_OK               0
#define MCS_ERR_SYNTAX      -1
#define MCS_ERR_IO          -2
#define MCS_ERR_SECURITY    -3
#define MCS_ERR_TIMEOUT     -4
#define MCS_ERR_FEED        -5
#define MCS_ERR_SYSTEM      -6   // ← NEU — für fork/exec-Fehler

// Public API
int mcs_run_transaktion(mcs_transaktion_t* t);
int mcs_run_protein(mcs_protein_t* p);
int mcs_run_action(mcs_action_t* a);

#endif
