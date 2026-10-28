// Lib/mcs_cmd_register.c — v1.0 minimal, feed-focused
#include "mcs_cmd_register.h"
#include "mcs_feed.h"
#include <string.h>
#include <stdlib.h>

int mcs_cmd_execute(const char* cmd, const char* arg) {
    if (!cmd) return -1;

    // Einziger unterstützter Befehl: 'feed cache'
    if (strcmp(cmd, "feed cache") == 0) {
        if (!arg || arg[0] == '\0') return -1;
        int id = atoi(arg);
        if (id < 0 || id >= MCS_FEED_MAX) return -1;
        mcs_feed_clear(id);
        return 0; // MCS_OK
    }

    // Später: 'feed dump', 'feed set', 'sync', ...
    return -2; // MCS_ERR_CMD_UNKNOWN
}

void mcs_register_init(void) {
    // Platzhalter — später dynamisch erweiterbar
}
