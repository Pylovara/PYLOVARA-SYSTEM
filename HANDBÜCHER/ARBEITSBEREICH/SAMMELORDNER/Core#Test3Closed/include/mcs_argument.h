// include/mcs_argument.h — v3.0, mit Einheiten & Sicherheitskontext
#ifndef MCS_ARGUMENT_H
#define MCS_ARGUMENT_H

#include "mcs_token.h"

// Einheiten für zeitliche/quantitative Argumente
typedef enum {
    UNIT_NONE = 0,
    UNIT_SECONDS,
    UNIT_MILLISECONDS,
    UNIT_PERCENT,
    UNIT_ABSOLUTE    // z.B. PID, Thread-ID, Watt
} mcs_unit_t;

// Argument-Typen (gemäß @kernel 15, erweitert um Semantik)
typedef enum {
    ARG_NONE = 0,
    ARG_MINUS,    // «««- → Subtraktion
    ARG_PLUS,     // «««+ → Addition
    ARG_MUL,      // «««· → Multiplikation
    ARG_DIV,      // «««: → Division
    ARG_PERCENT,  // «««%N → Prozent (0–100)
    ARG_TIME,     // «««tN → Zeit (ms/s)
    ARG_REBOOT,   // «««rN → Reboot-Verzögerung
    ARG_KILL,     // «««kN → SIGKILL nach N Sekunden (später)
    ARG_THROTTLE  // «««wN → Watt-Begrenzung (später)
} mcs_arg_type_t;

// Argument-Container mit Semantik
typedef struct {
    mcs_arg_type_t type;
    mcs_unit_t     unit;
    double         value;      // double für %0.5, 1.5s, etc.
    int            min_value;  // Sicherheitslimit: z.B. ARG_TIME ≥ 0
    int            max_value;  // z.B. ARG_PERCENT ≤ 100
    const char*    context;    // z.B. "CPU", "NET", "STORAGE"
} mcs_argument_t;

// Public API
int mcs_parse_argument(mcs_lexer_t* lex, mcs_argument_t* out);
int mcs_validate_argument(const mcs_argument_t* a);
double mcs_resolve_value(const mcs_argument_t* a, int system_load); // Context-Aware

#endif
