// include/mcs_feed.h — MCS Feed Registry (v2.9)
// include/mcs_feed.h — v3.0 RAM-gepuffert, debugbar, sicher
#ifndef MCS_FEED_H
#define MCS_FEED_H

#include <stddef.h>

#define MCS_FEED_MAX 256

typedef struct {
    int id;
    char* value;       // malloc'ed — RAM-basiert
    size_t value_len;  // Länge — für Sicherheit
    int is_set;
    const char* source; // Debug: woher kam das? z.B. "$pgrep firefox"
} mcs_feed_t;

// RAM-Initialisierung — MUSS VOR ALLEM ANDEREN aufgerufen werden!
void mcs_feed_init(void);

// Public API — wie vorher, nur sicherer
int  mcs_feed_set(int id, const char* value, const char* source);
const char* mcs_feed_get(int id);
void mcs_feed_clear(int id);
void mcs_feed_reset_all(void);

// Debug-Hilfe
void mcs_feed_dump(void); // gibt alle Feeds aus

#endif
