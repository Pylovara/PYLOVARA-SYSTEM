// Logic/mcs_lexer.h
#ifndef MCS_LEXER_H
#define MCS_LEXER_H

#include "mcs_token.h"   // ← Grundtypen: mcs_token_t, mcs_token_type_t, TOK_*

// 🔹 Opaque Lexer — Definition bleibt in mcs_lexer.c
typedef struct mcs_lexer mcs_lexer_t;

// 🔹 Public API — NUR Deklarationen
mcs_lexer_t* mcs_lexer_new(const char* input);
void mcs_lexer_free(mcs_lexer_t* lex);
mcs_token_t mcs_lexer_next(mcs_lexer_t* lex);

// 🔹 NEU: Lookahead — liest, bewegt aber nicht
mcs_token_t mcs_lexer_peek(mcs_lexer_t* lex);

// 🔹 Safe Read-Only Access — KEIN direkter Member-Zugriff erlaubt!
const char* mcs_lexer_get_current_pos(mcs_lexer_t* lex);
int mcs_lexer_get_line(mcs_lexer_t* lex);
int mcs_lexer_get_column(mcs_lexer_t* lex);

#endif // MCS_LEXER_H
