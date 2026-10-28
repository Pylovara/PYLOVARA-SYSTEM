// OrdnerPfad: /Pylovara/MCSKernel/Lib/mcs-ausfuehrung.c

#include "mcs_kernel_core.h"
#include "mcs_lexer.h"
#include "mcs_alu_prozessor.h"
#include <stdio.h>

void mcs_prozess_validieren(Token *t) {
    // Ebene 1: Sentiator ſ prüft Reinheit
    if (t->typ == TOKEN_REINHEIT) {
        printf("ſ [REINHEITS_CHECK]: Validiere Transaktionsrahmen...\n");
    }

    // Ebene 2: Fortschritt – KORREKTUR: TOKEN_FOR_PROGRESS statt TOKEN_FORTSCHRITT
    if (t->typ == TOKEN_FOR_PROGRESS) {
        printf("– [GATTER_SCHALTUNG]: Fortschritt erkannt. Schalte Pfad frei.\n");
    }
}

void mcs_aktion_materialisieren(Token *t) {
    // Ebene 3: Aktion » erlaubt erst hier die physische Tat
    if (t->typ == TOKEN_AKTION_START) {
        ALU_TAKT_SYNC(); // Jetzt findet der Compiler die Definition aus dem Header
        printf("» [AKTION]: Befehl wird ins Silizium geschrieben.\n");
    }
}
