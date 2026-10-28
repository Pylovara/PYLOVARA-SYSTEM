// Lib/mcs_wahrheiten.c — v3.0-konform, containerbasiert, fehlerfrei
#include "mcs_wahrheiten.h"

int mcs_token_is_wahrheit(mcs_token_type_t tt) {
    return tt == TOK_IF || tt == TOK_ELSE ||
    tt == TOK_WHEN_NOT || tt == TOK_MUST ||
    tt == TOK_ON_ERROR || tt == TOK_PARALLEL_RUN ||
    tt == TOK_PARALLEL_TRANSPORT;
}

mcs_wahrheit_t mcs_token_to_wahrheit(mcs_token_type_t tt) {
    switch (tt) {
        case TOK_IF:                    return WAHR_IF;
        case TOK_ELSE:                  return WAHR_ELSE;
        case TOK_WHEN_NOT:              return WAHR_WHEN_NOT;
        case TOK_MUST:                  return WAHR_MUST;
        case TOK_ON_ERROR:              return WAHR_ON_ERROR;
        case TOK_PARALLEL_RUN:          return WAHR_PARALLEL_RUN;
        case TOK_PARALLEL_TRANSPORT:    return WAHR_PARALLEL_TRANSPORT;
        default:                        return WAHR_NONE;
    }
}
