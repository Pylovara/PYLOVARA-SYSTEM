// Lib/mcs_protein.c — v3.0 MVP: IF/ELSE container-basiert
#include <stdlib.h>
#include "mcs_protein.h"
#include "mcs_wahrheiten.h"

// 🔹 Forward-Deklaration für rekursive Nutzung
static mcs_protein_t* mcs_parse_protein_from_lexer(mcs_lexer_t* lex);

// 🔹 EINZIGE Implementierung von mcs_free_protein — KEINE DUPLIKATE!
void mcs_free_protein(mcs_protein_t* p) {
    if (!p) return;
    if (p->action) mcs_free_action(p->action);
    if (p->then_branch) mcs_free_protein(p->then_branch);
    if (p->else_branch) mcs_free_protein(p->else_branch);
    free(p);
}

mcs_protein_t* mcs_parse_protein(const char* input) {
    if (!input) return NULL;

    mcs_lexer_t* lex = mcs_lexer_new(input);
    if (!lex) return NULL;

    mcs_protein_t* p = calloc(1, sizeof(mcs_protein_t));
    if (!p) {
        mcs_lexer_free(lex);
        return NULL;
    }

    mcs_token_t tok = mcs_lexer_next(lex);

    // 🔹 Fall 1: Wahrheit (IF/ELSE)
    if (mcs_token_is_wahrheit(tok.token_type)) {
        p->wahrheit = mcs_token_to_wahrheit(tok.token_type);

        if (p->wahrheit == WAHR_IF) {
            p->then_branch = mcs_parse_protein_from_lexer(lex);
            // ELSE später mit Lookahead
        }
    }
    // 🔹 Fall 2: Normale Aktion »[…]«
    else if (tok.token_type == TOK_ACTION_START) {
        mcs_lexer_free(lex);
        p->action = mcs_parse_action(input);
    } else {
        mcs_lexer_free(lex);
        free(p);
        return NULL;
    }

    mcs_lexer_free(lex);
    return p;
}

// 🔹 Helper — bleibt unten, nach der Hauptfunktion
static mcs_protein_t* mcs_parse_protein_from_lexer(mcs_lexer_t* lex) {
    mcs_token_t tok = mcs_lexer_next(lex);
    if (tok.token_type == TOK_ACTION_START) {
        return calloc(1, sizeof(mcs_protein_t));
    }
    return NULL;
}
