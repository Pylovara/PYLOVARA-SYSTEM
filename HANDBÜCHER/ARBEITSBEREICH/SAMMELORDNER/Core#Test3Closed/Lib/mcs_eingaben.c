// Lib/mcs_eingaben.c
#include <stdlib.h>
#include <string.h>
#include "mcs_eingaben.h"
#include "mcs_token.h"

int mcs_is_eingabe_quote(mcs_token_type_t tt) {
    return tt == TOK_STR_DBL_QUOTE || tt == TOK_STR_SGL_QUOTE ||
    tt == TOK_STR_TICK_QUOTE || tt == TOK_BLANKERNER_QUOTE;
}

mcs_eingabe_typ_t mcs_token_to_eingabe_typ(mcs_token_type_t tt) {
    switch (tt) {
        case TOK_STR_DBL_QUOTE:   return MCS_EINGABE_PRINT;
        case TOK_STR_SGL_QUOTE:   return MCS_EINGABE_SYSTEM_CMD;
        case TOK_STR_TICK_QUOTE:  return MCS_EINGABE_MCS_CMD;
        case TOK_BLANKERNER_QUOTE:return MCS_EINGABE_BLANKERNER;
        default:                  return MCS_EINGABE_NOTIZ;
    }
}

// MVP: Nur Speicher allozieren, kein Parsing — später: Content extrahieren
mcs_eingabe_t* mcs_parse_eingabe_from_token(mcs_token_type_t quote_typ, const char* start, int len) {
    mcs_eingabe_t* e = calloc(1, sizeof(mcs_eingabe_t));
    if (!e) return NULL;

    e->typ = mcs_token_to_eingabe_typ(quote_typ);
    e->laenge = len;
    if (len > 0) {
        e->inhalt = strndup(start, len);
        e->quote_start = 0;
        e->quote_end = len + 1; // Platzhalter
    }
    return e;
}

void mcs_free_eingabe(mcs_eingabe_t* e) {
    if (!e) return;
    free(e->inhalt);
    free(e);
}
