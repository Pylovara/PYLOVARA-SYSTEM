#include "mcs_wahrheiten.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Visualisierung des Protein-Ladevorgangs
void mcs_progress_operator(int stufe, int prozent) {
    printf("  --");
    for(int i=0; i<stufe; i++) printf("·");
    printf(": ");
    int balken = prozent / 3.2;
    for(int i=0; i<31; i++) {
        if(i < balken) printf("#"); else printf(" ");
    }
    printf(" %d%% »[AKTIV]«\n", prozent);
}

// Prüft, ob ein Feed (Register) einen "Wahr-Impuls" enthält
int mcs_ist_wahr(const char *wert) {
    if (!wert || strlen(wert) == 0) return 0;

    // 1. Numerische Prüfung: "1" oder jede Zahl > 0 ist WAHR
    if (strcmp(wert, "1") == 0 || atoi(wert) > 0) {
        return 1;
    }

    // 2. Textuelle Prüfung
    if (strcasecmp(wert, "OK") == 0 ||
        strcasecmp(wert, "true") == 0 ||
        strcasecmp(wert, "SUCCESS") == 0) {
        return 1;
        }

        return 0; // Alles andere ist neutral (falsch)
}
