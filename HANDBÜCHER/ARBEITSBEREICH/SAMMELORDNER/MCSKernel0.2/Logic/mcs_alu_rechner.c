#include "mcs_alu_rechner.h"
#include "mcs_register.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static long mcs_alu_get_val(const char *s) {
    if (!s) return 0;
    char clean_buffer[64];
    int j = 0;

    // Wir nehmen NUR Ziffern. Alles andere (¨, Leerzeichen) fliegt raus.
    for (int i = 0; s[i] != '\0' && j < 63; i++) {
        if (isdigit((unsigned char)s[i])) {
            clean_buffer[j++] = s[i];
        }
    }
    clean_buffer[j] = '\0';
    return (j > 0) ? atol(clean_buffer) : 0;
}

long mcs_alu_berechne(const char *ausdruck) {
    if (!ausdruck) return 0;

    printf("[ALU-TAKT]: Verarbeite Box-Inhalt: %s\n", ausdruck);

    // 1. Bekannte Wahrheiten abfangen
    if (strstr(ausdruck, "true") || strstr(ausdruck, "valid")) return 1;
    if (strstr(ausdruck, "false") || strstr(ausdruck, "error")) return 0;

    // 2. Operatoren suchen
    const char* ops[] = {">", "<", "==", "!="};
    for(int i=0; i<4; i++) {
        char* op_pos = strstr(ausdruck, ops[i]);
        if (op_pos) {
            char left[128] = {0}, right[128] = {0};
            int op_len = strlen(ops[i]);

            // Split in Links und Rechts vom Operator
            int left_len = op_pos - ausdruck;
            strncpy(left, ausdruck, left_len);
            left[left_len] = '\0';
            strcpy(right, op_pos + op_len);

            printf("[ALU-TAKT]: Vergleich erkannt: [%s] %s [%s]\n", left, ops[i], right);

            long v1 = mcs_alu_get_val(left);
            long v2 = mcs_alu_get_val(right);

            long res = 0;
            if (i == 0) res = (v1 > v2);
            else if (i == 1) res = (v1 < v2);
            else if (i == 2) res = (v1 == v2);
            else if (i == 3) res = (v1 != v2);

            printf("[ALU-RESULTAT]: %ld %s %ld = %ld\n", v1, ops[i], v2, res);
            return res;
        }
    }

    // 3. Fallback: Multiplikation oder Einzelwert
    if (strchr(ausdruck, '*')) {
        long res = 1;
        char temp[256];
        strncpy(temp, ausdruck, 255);
        char *token = strtok(temp, "*");
        while (token) {
            res *= mcs_alu_get_val(token);
            token = strtok(NULL, "*");
        }
        return res;
    }

    return mcs_alu_get_val(ausdruck);
}
