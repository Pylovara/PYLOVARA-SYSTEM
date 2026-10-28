// Lib/mcs_feed.c — v3.0 RAM-gepuffert
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mcs_feed.h"

static mcs_feed_t feed_table[MCS_FEED_MAX] = {{0}};
static int feed_is_initialized = 0;

void mcs_feed_init(void) {
    if (feed_is_initialized) return;
    for (int i = 0; i < MCS_FEED_MAX; i++) {
        feed_table[i].id = i;
        feed_table[i].value = NULL;
        feed_table[i].value_len = 0;
        feed_table[i].is_set = 0;
        feed_table[i].source = NULL;
    }
    feed_is_initialized = 1;
    fprintf(stderr, "[FEED] RAM-Cache initialisiert (%d Einträge)\n", MCS_FEED_MAX);
}

int mcs_feed_set(int id, const char* value, const char* source) {
    if (!feed_is_initialized) mcs_feed_init();
    if (id < 0 || id >= MCS_FEED_MAX) return -1;

    mcs_feed_t* f = &feed_table[id];
    free(f->value);
    f->value = value ? strdup(value) : NULL;
    f->value_len = value ? strlen(value) : 0;
    f->is_set = (value != NULL);
    f->source = source ? strdup(source) : NULL;
    return 0;
}

const char* mcs_feed_get(int id) {
    if (!feed_is_initialized) return NULL;
    if (id < 0 || id >= MCS_FEED_MAX) return NULL;
    mcs_feed_t* f = &feed_table[id];
    return f->is_set ? f->value : NULL;
}

void mcs_feed_clear(int id) {
    if (!feed_is_initialized) return;
    if (id < 0 || id >= MCS_FEED_MAX) return;
    mcs_feed_t* f = &feed_table[id];
    free(f->value);
    free((void*)f->source);
    f->value = NULL;
    f->value_len = 0;
    f->is_set = 0;
    f->source = NULL;
}

void mcs_feed_reset_all(void) {
    if (!feed_is_initialized) return;
    for (int i = 0; i < MCS_FEED_MAX; i++) {
        mcs_feed_clear(i);
    }
}

void mcs_feed_dump(void) {
    if (!feed_is_initialized) {
        fprintf(stderr, "[FEED] Nicht initialisiert!\n");
        return;
    }
    fprintf(stderr, "[FEED] Zustand:\n");
    for (int i = 0; i < MCS_FEED_MAX; i++) {
        if (feed_table[i].is_set) {
            fprintf(stderr, "  (%d): '%s' ← '%s'\n",
                    i, feed_table[i].value, feed_table[i].source);
        }
    }
}
