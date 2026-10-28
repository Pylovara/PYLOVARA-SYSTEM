#include "mcs_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static void mcs_token_wert_setzen(Token *t, const char *quell, int laenge) {
    if (laenge >= 64) laenge = 63;
    strncpy(t->wert, quell, laenge);
    t->wert[laenge] = '\0';
}

Token* mcs_lexer_erzeuge(const char *quellcode) {
    Token *tokens = malloc(8192 * sizeof(Token));
    int index = 0;
    const char *p = quellcode;

    while (*p) {
        if (isspace(*p)) { p++; continue; }

        // 1. KOMMENTARE
        if (*p == '#') {
            while (*p && *p != '\n') p++;
            continue;
        }

        // 2. TRANSAKTIONS-RAHMEN (¢! / !¢)
        if ((unsigned char)p[0] == 0xC2 && (unsigned char)p[1] == 0xA2 && p[2] == '!') {
            tokens[index].typ = TOKEN_TRANS_START;
            mcs_token_wert_setzen(&tokens[index++], "¢!", 3);
            p += 3; continue;
        }
        if (p[0] == '!' && (unsigned char)p[1] == 0xC2 && (unsigned char)p[2] == 0xA2) {
            tokens[index].typ = TOKEN_TRANS_ENDE;
            mcs_token_wert_setzen(&tokens[index++], "!¢", 3);
            p += 3; continue;
        }

        // 3. SENTIATOREN (¶ / ¶¶)
        if ((unsigned char)p[0] == 0xC2 && (unsigned char)p[1] == 0xB6) {
            if ((unsigned char)p[2] == 0xC2 && (unsigned char)p[3] == 0xB6) {
                tokens[index].typ = TOKEN_PARA_NOT;
                mcs_token_wert_setzen(&tokens[index++], "¶¶", 4);
                p += 4;
            } else {
                tokens[index].typ = TOKEN_PARA_IF;
                mcs_token_wert_setzen(&tokens[index++], "¶", 2);
                p += 2;
            }
            continue;
        }

        // 4. ALU-BOX (×) - Priorität 1
        // Prüft ob wir ein × sehen (0xC3 0x97)
        if ((unsigned char)p[0] == 0xC3 && (unsigned char)p[1] == 0x97) {
            p += 2; // Start-× überspringen
            const char *start = p;
            while (*p && !((unsigned char)p[0] == 0xC3 && (unsigned char)p[1] == 0x97)) p++;

            tokens[index].typ = TOKEN_BOXI_ALU;
            mcs_token_wert_setzen(&tokens[index++], start, p - start);

            if ((unsigned char)p[0] == 0xC3 && (unsigned char)p[1] == 0x97) p += 2; // End-× überspringen
            continue;
        }

        // 6. SHELL-BOX (') - Priorität 2
        if (*p == '\'') {
            p++;
            const char *start = p;
            while (*p && *p != '\'') p++;
            tokens[index].typ = TOKEN_BOXI_SHELL;
            mcs_token_wert_setzen(&tokens[index++], start, p - start);
            if (*p == '\'') p++;
            continue;
        }

        // 8. ALLGEMEINE BOX ([, aber NICHT wenn danach ein × kommt)
        if (p[0] == '[') {
            // Spezialfall: Wenn direkt nach [ ein × kommt, ist es die ALU-Box.
            // Die lassen wir dann im nächsten Durchlauf von Punkt 4 fangen.
            if ((unsigned char)p[1] == 0xC3 && (unsigned char)p[2] == 0x97) {
                p++; // Nur die [ überspringen
                continue;
            }

            p++;
            const char *start = p;
            int klammern = 1;
            while (*p && klammern > 0) {
                if (*p == '[') klammern++;
                if (*p == ']') klammern--;
                if (klammern > 0) p++;
            }
            tokens[index].typ = TOKEN_BOXI_SHELL;
            mcs_token_wert_setzen(&tokens[index++], start, p - start);
            if (*p == ']') p++;
            continue;
        }

        // 5. AKTION-RAHMEN (» / «)
        if ((unsigned char)p[0] == 0xC2 && (unsigned char)p[1] == 0xBB) {
            tokens[index].typ = TOKEN_AKTION_START;
            mcs_token_wert_setzen(&tokens[index++], "»", 2);
            p += 2; continue;
        }
        if ((unsigned char)p[0] == 0xC2 && (unsigned char)p[1] == 0xAB) {
            tokens[index].typ = TOKEN_AKTION_ENDE;
            mcs_token_wert_setzen(&tokens[index++], "«", 2);
            p += 2; continue;
        }

        // 7. ZUWEISUNG (-=)
        if (strncmp(p, "-=", 2) == 0) {
            tokens[index].typ = TOKEN_ASSIGN;
            mcs_token_wert_setzen(&tokens[index++], "-=", 2);
            p += 2; continue;
        }

        // 9. FORTSCHRITT (–)
        if ((unsigned char)p[0] == 0xE2 && (unsigned char)p[1] == 0x80 && (unsigned char)p[2] == 0x93) {
            tokens[index].typ = TOKEN_FOR_PROGRESS;
            mcs_token_wert_setzen(&tokens[index++], "–", 3);
            p += 3; continue;
        }

        p++; // Fallback
    }

    tokens[index].typ = TOKEN_LEX_EOF;
    return tokens;
}
