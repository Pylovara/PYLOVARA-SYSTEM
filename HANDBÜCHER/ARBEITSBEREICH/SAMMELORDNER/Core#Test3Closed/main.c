// main.c — MCS v3.0 Runner (UTF-8-konform, sicher, modular)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcs_token.h"
#include "mcs_transaktion.h"
#include "mcs_runner.h"
#include "mcs_feed.h"   // ← für mcs_feed_init()

void print_usage(const char* prog) {
    fprintf(stderr, "Usage: %s <file.mcs>\n", prog);
    exit(1);
}

int main(int argc, char* argv[]) {
    // 🔹 1. RAM-Initialisierung — MUSS ZUERST!
    mcs_feed_init();

    // 🔹 2. Argument-Check
    if (argc != 2) print_usage(argv[0]);

    const char* filename = argv[1];
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "[ERROR] Cannot open '%s'\n", filename);
        return 1;
    }

    // 🔹 3. Datei einlesen
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = malloc(size + 1);
    if (!buf) { perror("malloc"); fclose(f); return 1; }
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);

    // 🔹 4. Parsen
    mcs_transaktion_t* t = mcs_parse_transaktion(buf);
    if (!t || !t->is_valid) {
        fprintf(stderr, "[PARSE ERROR] Invalid MCS syntax in '%s'\n", filename);
        free(buf);
        return 1;
    }

    // 🔹 5. Ausführen
    int result = mcs_run_transaktion(t);
    if (result != MCS_OK) {
        fprintf(stderr, "[RUNTIME ERROR] Code %d\n", result);
    } else {
        printf("✅ MCS executed successfully.\n");
    }

    // 🔹 6. Aufräumen
    mcs_free_transaktion(t);
    free(buf);
    return result;
}
