// include/mcs_wahrheit.h — v3.0, *nur* Deklaration
#ifndef MCS_WAHRHEIT_H
#define MCS_WAHRHEIT_H

#include "mcs_token.h"   // ← für mcs_token_type_t

// 🔹 Enum muss *vor* Funktionsdeklaration kommen!
typedef enum {
    WAHR_NONE = 0,
    WAHR_IF,          // ¶
    WAHR_ELSE,        // ¶¶
    WAHR_WHEN_NOT,    // °
    WAHR_MUST,        // !
    WAHR_ON_ERROR,    // …
    WAHR_PARALLEL_RUN,      // ¶=
    WAHR_PARALLEL_TRANSPORT // ;;
} mcs_wahrheit_t;

// 🔹 Nur Deklarationen — KEINE Implementierung!
int mcs_token_is_wahrheit(mcs_token_type_t tt);
mcs_wahrheit_t mcs_token_to_wahrheit(mcs_token_type_t tt);

#endif // MCS_WAHRHEIT_H
