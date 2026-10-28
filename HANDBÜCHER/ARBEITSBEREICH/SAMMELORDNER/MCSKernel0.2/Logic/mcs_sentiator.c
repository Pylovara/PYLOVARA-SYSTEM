#include "mcs_sentiator.h"
#include "mcs_register.h"
#include "mcs_wahrheiten.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mcs_sentiator_evaluiere_boxi(int typ, const char *aufgeloest) {
    if (!aufgeloest) return 0;

    // Nutzt mcs_ist_wahr (prüft jetzt "1", "OK", etc.)
    int ist_wahr = mcs_ist_wahr(aufgeloest);

    switch(typ) {
        case 1: // ¶ SENTIATOR-KANN (Wahr-Impuls)
            if (ist_wahr) {
                printf("  ¶ [SENTIATOR-KANN]: Impuls bestätigt. Signal: [%s]\n", aufgeloest);
                return 1;
            }
            if (strlen(aufgeloest) > 0) {
                printf("  [GATE]: Sperrung aktiv. Protein '%s' ist neutral.\n", aufgeloest);
            }
            return 0;

        case 2: // ¶¶ SENTIATOR-NICHT (Negativ-Impuls)
            if (!ist_wahr) {
                printf("  ¶¶ [SENTIATOR-NICHT]: Negativ-Impuls bestätigt. Inhalt neutral.\n");
                return 1;
            }
            printf("  [GATE]: Sperrung aktiv. Signal [%s] vorhanden, Negativ-Gatter schließt.\n", aufgeloest);
            return 0;

        case 6: // ſ REINHEIT (Datenmüll-Detektor laut System-00.48)
            if (strstr(aufgeloest, "ERR") || strstr(aufgeloest, "TRASH")) {
                printf("  ſ [REINHEIT]: Datenmüll erkannt: %s\n", aufgeloest);
                return 1;
            }
            return 0;

        default:
            return 0;
    }
}
