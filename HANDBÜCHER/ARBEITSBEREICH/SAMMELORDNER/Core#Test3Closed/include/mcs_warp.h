// include/mcs_warp.h
#ifndef MCS_WARP_H
#define MCS_WARP_H

typedef enum {
    WARP_C, WARP_CSS, WARP_JS, WARP_PY, WARP_ASM, WARP_UNKNOWN
} mcs_warp_lang_t;

typedef struct {
    mcs_warp_lang_t lang;
    char* code;        // z.B. "#include <stdio.h>..."
    char* target;      // z.B. "$gcc", "§kitty"
} mcs_warp_t;

mcs_warp_t* mcs_parse_warp(const char* input);
void mcs_free_warp(mcs_warp_t* w);

#endif
