#include "mcs_feed.h"
#include "mcs_register.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    char cache[256];
    int aktiv;
} MCS_Feed;

static MCS_Feed feed_speicher[17]; // Indizes 1-16

void mcs_feed_laden(int id, const char* inhalt) {
    if (id < 1 || id > 16) return;

    strncpy(feed_speicher[id].cache, inhalt ? inhalt : "0", 255);
    feed_speicher[id].aktiv = 1;

    // Synchronisierung mit dem permanenten Register (FF)
    mcs_register_setze(id, feed_speicher[id].cache);

    printf("    [FEED-SYSTEM]: Feed(%d) mit Ladung gefüllt: %s\n", id, feed_speicher[id].cache);
}

const char* mcs_feed_lesen(int id) {
    if (id < 1 || id > 16) return "0";

    // Falls der Cache im aktuellen Takt gefüllt wurde, nimm diesen
    if (feed_speicher[id].aktiv) {
        return feed_speicher[id].cache;
    }

    // Fallback auf das Register
    return mcs_register_hole(id);
}
