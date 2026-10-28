// Lib/mcs_action.c — v3.0 Parser-only (Runner-Logik kommt später!)
#include <stdlib.h>
#include <string.h>
#include "mcs_action.h"
#include "mcs_argument.h"

mcs_action_t* mcs_parse_action(const char* input) {
    if (!input) return NULL;

    mcs_lexer_t* lex = mcs_lexer_new(input);
    if (!lex) return NULL;

    mcs_action_t* a = calloc(1, sizeof(mcs_action_t));
    if (!a) {
        mcs_lexer_free(lex);
        return NULL;
    }

    mcs_token_t tok;

    // »
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_ACTION_START) goto fail;

    // [
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_PROTEIN_OPEN) goto fail;

    // 'cmd'
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_STR_SGL_QUOTE) goto fail;
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_ID) goto fail;
    a->cmd = token_to_string(tok);

    // |
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_SEP) goto fail;

    // (1) — optional
    tok = mcs_lexer_next(lex);
    if (tok.token_type == TOK_FEED_OPEN) {
        tok = mcs_lexer_next(lex);
        a->feed.id = extract_number(tok);
        a->feed.is_set = (a->feed.id > 0);
        tok = mcs_lexer_next(lex); // )
    }

    // ¨...¨ oder "..." — arg1 / blankernenner
    if (tok.token_type == TOK_BLANKERNER_QUOTE) {
        tok = mcs_lexer_next(lex);
        if (tok.token_type == TOK_ID || tok.token_type == TOK_LITERAL) {
            a->blankernenner = token_to_string(tok);
        }
    } else if (tok.token_type == TOK_STR_DBL_QUOTE) {
        tok = mcs_lexer_next(lex);
        if (tok.token_type == TOK_ID || tok.token_type == TOK_NUMBER) {
            a->arg1 = token_to_string(tok);
        }
    }

    // ]
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_PROTEIN_CLOSE) goto fail;

    // « — Aktion-Ende
    tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_ACTION_END) goto fail;

    // ✅ JETZT: Argumente *parsen* — aber NICHT auswerten!
    mcs_parse_argument(lex, &a->arg);
    if (!mcs_validate_argument(&a->arg)) {
        goto fail;
    }

    mcs_lexer_free(lex);
    return a;

    fail:
    mcs_lexer_free(lex);
    if (a) {
        free(a->cmd);
        free(a->arg1);
        free(a->blankernenner);
        free(a);
    }
    return NULL;
}

void mcs_free_action(mcs_action_t* a) {
    if (!a) return;
    free(a->cmd);
    free(a->arg1);
    free(a->blankernenner);
    free(a);
}
