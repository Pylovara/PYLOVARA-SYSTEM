// Lab/test_if.c — v3.0-konform, fehlerfrei
#include <stdio.h>
#include "mcs_token.h"        // ← NICHT mcs_lexer.h!
#include "mcs_wahrheiten.h"     // ← NICHT mcs_wahrheiten.h (kein -en!)

int main() {
    const char input[] = "¶ ¶¶";
    mcs_lexer_t* lex = mcs_lexer_new(input);
    if (!lex) return 1;

    mcs_token_t tok;

    tok = mcs_lexer_next(lex);
    printf("Token 1: %s\n",
           mcs_token_is_wahrheit(tok.token_type) ? "WAHRHEIT (IF)" : "OTHER");

    tok = mcs_lexer_next(lex);
    printf("Token 2: %s\n",
           mcs_token_is_wahrheit(tok.token_type) ? "WAHRHEIT (ELSE)" : "OTHER");

    mcs_lexer_free(lex);
    return 0;
}
