#include "mcs_register.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 17 Register (0-16), jedes kann 255 Zeichen Text/Protein speichern
static char MCS_REGISTER_GATTER[17][256] = {"0"};

void ALU_REINIGEN_B64() {
    for (int i = 0; i <= 16; i++) strcpy(MCS_REGISTER_GATTER[i], "0");
}

// Die Brücke für die ALU (wandelt Protein-String in Rechenwert)
long mcs_register_hole_num(int reg_nr) {
    if (reg_nr >= 0 && reg_nr <= 16) {
        return atol(MCS_REGISTER_GATTER[reg_nr]);
    }
    return 0;
}

void mcs_register_setze(int reg_nr, const char *wert_str) {
    if (wert_str == NULL) return;
    int ziel_reg = reg_nr;

    // Automatisches Finden eines freien Registers, falls 0 angegeben
    if (reg_nr == 0) {
        for (int i = 1; i <= 16; i++) {
            if (strcmp(MCS_REGISTER_GATTER[i], "0") == 0) {
                ziel_reg = i;
                break;
            }
        }
    }

    if (ziel_reg >= 0 && ziel_reg <= 16) {
        strncpy(MCS_REGISTER_GATTER[ziel_reg], wert_str, 255);
        printf("    [TRANS-OK]: Protein '%s' fest an FF(%d) gebunden.\n", wert_str, ziel_reg);
    }
}

// Rückgabe des echten Strings (wichtig für Shell-Injektion)
char* mcs_register_hole(int reg_nr) {
    if (reg_nr >= 0 && reg_nr <= 16) {
        return MCS_REGISTER_GATTER[reg_nr];
    }
    return "0";
}

void mcs_register_status_anzeigen() {
    printf("\n--- PROTEIN-STATUS NACH TAKT-ENDE ---\n");
    for (int i = 1; i <= 3; i++) {
        printf(" FF(%d): %s\n", i, MCS_REGISTER_GATTER[i]);
    }
    printf("--------------------------------------\n");
}
