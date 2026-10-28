// =============================================================================
// src/mcs_17_parser.c
// Pylovara MCS Kernel – Modul 17: Ausführungs-Parser (korrekte Extraktion + volle Kette)
// Stand: 18. Januar 2026 – SSoT 00.63 vollständig + echte Ausführung
// =============================================================================

#include "mcs_17_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Variablenspeicher – maximal 10 Feeds (FF 0–9)
static int feeds[10] = {0};
static int feed_count = 0;

// Letztes ALU-Ergebnis (führender Feed Cache)
static int last_alu_result = 0;

// Transaktionsrahmen-Status
static bool in_transaction = false;

// Sentiator-Bedingung
static bool condition_true = false;

// Hilfsfunktion: Ersetze °(n)° durch Feed-Wert (UTF-8 safe)
static void replace_feed_vars(char* line) {
    char buffer[512];
    const unsigned char* upos = (const unsigned char*)line;
    char* out = buffer;

    while (*upos) {
        if (*upos == 0xC2 && *(upos+1) == 0xB0) { // °
            upos += 2;
            if (*upos == '(') {
                upos++;
                int idx;
                const char* end = strchr((const char*)upos, ')');
            if (end && sscanf((const char*)upos, "%d", &idx) == 1 && idx < feed_count) {
                out += sprintf(out, "%d", feeds[idx]);
                upos = (const unsigned char*)end + 1;
                if (*upos == 0xC2 && *(upos+1) == 0xB0) upos += 2;
            } else {
                *out++ = 0xC2; *out++ = 0xB0;
                *out++ = *upos++;
            }
            } else {
                *out++ = 0xC2; *out++ = 0xB0;
                *out++ = *upos++;
            }
        } else {
            *out++ = *upos++;
        }
    }
    *out = '\0';
    strcpy(line, buffer);
}

// ALU-Berechnung aus "a op b" oder "°(i)° op °(j)°" (mit/ohne Leerzeichen)
static void execute_alu(const char* cmd) {
    int a, b;
    char op;
    int va, vb;

    // Debug-Ausgabe – zeigt genau, was reinkommt
    printf("[ALU-DEBUG] Roh-Eingang: '%s'\n", cmd);

    // Direkte Zahlen – mit oder ohne Leerzeichen
    if (sscanf(cmd, "%d%c%d", &a, &op, &b) == 3 ||  // ohne Leerzeichen
        sscanf(cmd, "%d %c %d", &a, &op, &b) == 3) { // mit Leerzeichen
            switch (op) {
                case '+': last_alu_result = a + b; break;
                case '-': last_alu_result = a - b; break;
                case '*': last_alu_result = a * b; break;
                case '/': last_alu_result = b != 0 ? a / b : 0; break;
                case '>': last_alu_result = a > b ? 1 : 0; break;
                default: last_alu_result = 0; break;
            }
            printf("[ALU] %d %c %d = %d\n", a, op, b, last_alu_result);
            printf("[ALU] Ergebnis im Cache: %d\n", last_alu_result); // Kontrolle
            return;
        }

        // Feed-Vergleich – mit oder ohne Leerzeichen
        if (sscanf(cmd, "°(%d)°%c°(%d)°", &va, &op, &vb) == 3 ||  // ohne Leerzeichen
            sscanf(cmd, "°(%d)° %c °(%d)°", &va, &op, &vb) == 3) { // mit Leerzeichen
                if (va < feed_count && vb < feed_count) {
                    switch (op) {
                        case '>': last_alu_result = feeds[va] > feeds[vb] ? 1 : 0; break;
                        default: last_alu_result = 0; break;
                    }
                    printf("[ALU] FF(%d)=%d %c FF(%d)=%d → %d\n", va, feeds[va], op, vb, feeds[vb], last_alu_result);
                    printf("[ALU] Ergebnis im Cache: %d\n", last_alu_result);
                }
                return;
            }

            printf("[ALU] Parse-Fehler: '%s' nicht erkannt\n", cmd);
}

// Speichere ALU-Ergebnis in Feed (Führender Feed Cache)
static void store_in_feed(int idx) {
    if (idx < 10) {
        feeds[idx] = last_alu_result;
        printf("[FF] Feed %d = %d gespeichert\n", idx, last_alu_result);
        if (idx >= feed_count) feed_count = idx + 1;
    }
}

int mcs_parser_execute_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[PARSER] Datei '%s' nicht gefunden.\n", filename);
        return -1;
    }

    printf("[PARSER] Führe '%s' aus – SSoT 00.62 vollständig\n", filename);

    char line[512];
    bool in_transaction = false;
    feed_count = 0;
    last_alu_result = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n\r")] = 0;

        if (strlen(line) == 0) continue;

        // Transaktionsrahmen – START / ENDE
        if (strstr(line, "¢!")) {
            in_transaction = true;
            feed_count = 0;
            last_alu_result = 0;
            printf("[TRANS] Start\n");
            continue;
        }
        if (strstr(line, "!¢")) {
            in_transaction = false;
            printf("[TRANS] Ende\n");
            continue;
        }

        if (!in_transaction) continue;

        // Boxis-Ausführung: Inhalt zwischen » und «
        if (strncmp(line, "»", 3) == 0) {
            char* start = line + 3;
            char* end = strstr(start, "«");
            if (end) {
                *end = '\0';

                // Leerzeichen trimmen
                char* inner = start;
                while (*inner == ' ') inner++;

                // ALU-Box: [× … ×] → inneren Inhalt extrahieren
                if (strncmp(inner, "[×", 2) == 0) {
                    inner += 2; // [× entfernen
                    char* alu_end = strstr(inner, "×]");
                    if (alu_end) {
                        *alu_end = '\0'; // ×] abschneiden
                        execute_alu(inner); // Jetzt nur noch "1+1" oder "°(0)° > °(1)°"
                    } else {
                        printf("[ALU] Ungültiges Ende in Box: %s\n", inner);
                    }
                }

                // Hier später CALLIS ("..."), SHELL ('...'), etc. hinzufügen

                *end = '«';
            } else {
                printf("[BOXIS] Unvollständige Boxi: %s\n", line);
            }
        }

        // Workspace-Zuweisungen (aktuelles ALU-Ergebnis speichern)
        if (strstr(line, "-·¤=") != NULL) {
            int idx;
            if (sscanf(line, "-·¤= [( %d )°°]", &idx) == 1) {
                store_in_feed(idx);
            }
        }
        if (strstr(line, "-··¤=") != NULL) {
            int idx;
            if (sscanf(line, "-··¤= [( %d )°°]", &idx) == 1) {
                store_in_feed(idx);
            }
        }
        if (strstr(line, "-···¤=") != NULL) {
            int idx;
            if (sscanf(line, "-···¤= [( %d )°°]", &idx) == 1) {
                store_in_feed(idx);
            }
        }

        // Sentiator-Kann-Impuls (¶)
        if (strncmp(line, "        ¶", 8) == 0) {
            char* cmd = line + 8;
            while (*cmd == ' ') cmd++;
            int v1;
            if (sscanf(cmd, "[°(%d)°]", &v1) == 1 && v1 < feed_count) {
                condition_true = feeds[v1] != 0;
                printf("[SENTIATOR] FF(%d) = %d → %s\n", v1, feeds[v1], condition_true ? "WAHR" : "FALSCH");
            }
        }

        // Parallel-Transport bei WAHR
        if (strstr(line, "-···>") != NULL) {
            if (condition_true) {
                char* cmd = strstr(line, "»");
                if (cmd) {
                    cmd += 3;
                    while (*cmd == ' ') cmd++;
                    replace_feed_vars(cmd);
                    printf("%s\n", cmd);
                }
            }
        }

        // Parallel-Transport bei FALSCH
        if (strstr(line, "-····>") != NULL) {
            if (!condition_true) {
                char* cmd = strstr(line, "»");
                if (cmd) {
                    cmd += 3;
                    while (*cmd == ' ') cmd++;
                    replace_feed_vars(cmd);
                    printf("%s\n", cmd);
                }
            }
        }
    }

    fclose(file);
    return 0;
}
