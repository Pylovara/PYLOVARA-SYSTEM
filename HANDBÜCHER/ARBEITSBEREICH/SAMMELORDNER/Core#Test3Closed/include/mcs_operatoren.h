// include/mcs_operatoren.h — v3.0: Operator-Semantik, NAMENHAFTER struct
#ifndef MCS_OPERATOREN_H
#define MCS_OPERATOREN_H

#include "mcs_protein.h"
#include "mcs_semantics.h"   // ← für mcs_truth_t

// 🔹 Enum — muss MCS_OP_* enthalten
typedef enum {
    MCS_OP_NONE = 0,
    MCS_OP_WHEN_NOT,    // °
    MCS_OP_WHEN_LT,     // <
    MCS_OP_WHEN_GT,     // >
    MCS_OP_TRANS_ERROR, // ^
    MCS_OP_DATA_RESIDUE // ſ
} mcs_op_type_t;

// 🔹 STRUCT MIT NAME — muss struct mcs_operator heißen!
struct mcs_operator {
    mcs_op_type_t type;
    union {
        struct { mcs_protein_t* cond; } not;
        struct { int left_id; int right_id; } cmp;
        int residue_id;
    } data;
    mcs_protein_t* then_branch;
    mcs_protein_t* else_branch;
};
// 🔹 Typedef — muss mcs_operator_t heißen!
typedef struct mcs_operator mcs_operator_t;

// Public API
mcs_operator_t* mcs_operator_new(void);
void mcs_operator_free(mcs_operator_t* op);
int mcs_operator_set_type(mcs_operator_t* op, mcs_op_type_t type);
int mcs_operator_bind_condition(mcs_operator_t* op, mcs_protein_t* p);
int mcs_operator_bind_comparison(mcs_operator_t* op, int id_a, int id_b);
int mcs_operator_bind_residue(mcs_operator_t* op, int feed_id);
int mcs_operator_set_branches(mcs_operator_t* op, mcs_protein_t* then, mcs_protein_t* elseb);

mcs_truth_t mcs_evaluate_operator(const mcs_operator_t* op);

#endif // MCS_OPERATOREN_H
