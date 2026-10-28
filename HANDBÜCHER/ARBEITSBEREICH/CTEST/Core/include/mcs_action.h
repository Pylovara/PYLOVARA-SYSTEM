// include/mcs_action.h — FINAL v3.0
#ifndef MCS_ACTION_H
#define MCS_ACTION_H

#include <stdlib.h>   // ← für atoi()
#include <string.h>   // ← für strndup()
#include "mcs_token.h"
#include "mcs_feed.h"
#include "mcs_argument.h"

typedef struct {
    char* cmd;
    char* arg1;
    mcs_feed_t feed;
    char* blankernenner;
    mcs_argument_t arg;
} mcs_action_t;

mcs_action_t* mcs_parse_action(const char* input);
void mcs_free_action(mcs_action_t* a);

// Inline-Hilfen — jetzt mit korrektem Scope
static inline char* token_to_string(mcs_token_t tok) {
    return tok.length > 0 ? strndup(tok.literal, tok.length) : NULL;
}
static inline int extract_number(mcs_token_t tok) {
    return (tok.token_type == TOK_NUMBER && tok.length > 0)
    ? atoi(tok.literal) : 0;
}

#endif
