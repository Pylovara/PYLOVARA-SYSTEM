// /Pylovara/MCSKernel/main.c - läd *.mcs dateien über mcs_kernel_takt
#include "mcs_lexer.h"
#include "mcs_kernel_core.h"
#include "mcs_wahrheiten.h"
#include "mcs_register.h"
#include <stdio.h>
#include <stdlib.h>

// Hilfsfunktion zum Laden der .mcs Datei in den Speicher
char* mcs_datei_laden(const char *dateiname) {
    FILE *f = fopen(dateiname, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    string[fsize] = 0;
    return string;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("PYLOVARA MCS KERNEL v4.6 - STATUS: BEREIT\n");
        return 1;
    }

    char *quellcode = mcs_datei_laden(argv[1]);
    if (!quellcode) {
        printf("❌ Fehler beim Laden von %s\n", argv[1]);
        return 1;
    }

    // Kernel-Ausführung
    mcs_kernel_takt(quellcode);

    // --- PROTEIN-ÜBERSICHT (Sicherheits-Variante) ---
    const char *r1 = mcs_register_hole(1);
    const char *r2 = mcs_register_hole(2);
    const char *r3 = mcs_register_hole(3);

    printf("\n--- PROTEIN-STATUS NACH TAKT-ENDE ---\n");
    printf(" FF(1): %s\n", r1 ? r1 : "(leer)");
    printf(" FF(2): %s\n", r2 ? r2 : "(leer)");
    printf(" FF(3): %s\n", r3 ? r3 : "(leer)");
    printf("--------------------------------------\n");

    free(quellcode);
    return 0;
}
