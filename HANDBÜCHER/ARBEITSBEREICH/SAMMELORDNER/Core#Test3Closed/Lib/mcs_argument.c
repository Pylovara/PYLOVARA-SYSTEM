// Lib/mcs_argument.c — v3.0, mit Kontext und Sicherheitslimits
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>   // Fehlte! Für INT_MIN, INT_MAX
#include "mcs_argument.h"

int mcs_parse_argument(mcs_lexer_t* lex, mcs_argument_t* out) {
    if (!lex || !out) return 0;
    *out = (mcs_argument_t){0};

    mcs_token_t tok = mcs_lexer_next(lex);
    if (tok.token_type != TOK_OP_MARK) return 0;  // «««

    tok = mcs_lexer_next(lex);
    if (tok.token_type == TOK_ARG_MINUS)   { out->type = ARG_MINUS;   }
    else if (tok.token_type == TOK_ARG_PLUS){ out->type = ARG_PLUS;    }
    else if (tok.token_type == TOK_ARG_MUL) { out->type = ARG_MUL;     }
    else if (tok.token_type == TOK_ARG_DIV) { out->type = ARG_DIV;     }
    else if (tok.token_type == TOK_ARG_PERCENT){ out->type = ARG_PERCENT; out->unit = UNIT_PERCENT; }
    else if (tok.token_type == TOK_ID && tok.length >= 2) {
        const char* s = tok.literal;
        if (s[0] == 't') {
            out->type = ARG_TIME;
            out->unit = UNIT_SECONDS;
            out->value = atof(s + 1);
        } else if (s[0] == 'r') {
            out->type = ARG_REBOOT;
            out->unit = UNIT_SECONDS;
            out->value = atof(s + 1);
        } else if (s[0] == '%') {
            out->type = ARG_PERCENT;
            out->unit = UNIT_PERCENT;
            out->value = atof(s + 1);
        }
    }

    // Sicherheitslimits
    switch (out->type) {
        case ARG_TIME: case ARG_REBOOT:
            out->min_value = 0;
            out->max_value = 86400;  // max 24h (später konfigurierbar)
            break;
        case ARG_PERCENT:
            out->min_value = 0;
            out->max_value = 100;
            break;
        default:
            out->min_value = INT_MIN;
            out->max_value = INT_MAX;
    }

    return mcs_validate_argument(out);
}

int mcs_validate_argument(const mcs_argument_t* a) {
    if (!a) return 0;
    if (a->value < a->min_value || a->value > a->max_value) return 0;
    if (a->type == ARG_PERCENT && a->unit != UNIT_PERCENT) return 0;
    if (a->type == ARG_TIME && a->unit != UNIT_SECONDS) return 0;
    return 1;
}

// Context-Aware Value Resolution (z.B. bei hoher CPU-Last → %50 = 30%, nicht 50%)
double mcs_resolve_value(const mcs_argument_t* a, int system_load) {
    if (!a) return 0.0;
    if (a->unit == UNIT_PERCENT) {
        // Dynamische Skalierung: bei 90% Last → 50% = 30% erlaubt
        double scale = 1.0 - (system_load / 200.0); // 0–100% Load → 1.0–0.5 Skalierung
        return a->value * scale;
    }
    return a->value;
}
