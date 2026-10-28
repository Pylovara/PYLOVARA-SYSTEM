#include "mcs_kernel_core.h"
#include "mcs_lexer.h"
#include "mcs_alu_rechner.h"
#include "mcs_register.h"
#include "mcs_sentiator.h"
#include "mcs_feed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Vorwärts-Deklarationen
void mcs_shell_ausfuehren(const char *cmd, char *output_buffer);
void mcs_inject_vars(char *dest, const char *src);

static char last_op_result[256] = "0";

// --- IMPLEMENTIERUNG HILFSFUNKTIONEN ---

void mcs_shell_ausfuehren(const char *cmd, char *output_buffer) {
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        strcpy(output_buffer, "0");
        return;
    }
    if (fgets(output_buffer, 256, fp) == NULL) {
        strcpy(output_buffer, "0");
    } else {
        output_buffer[strcspn(output_buffer, "\r\n")] = 0;
    }
    pclose(fp);
}

void mcs_inject_vars(char *dest, const char *src) {
    if (!src || !dest) return;
    char temp[1024] = {0};
    int t_idx = 0;
    int s_idx = 0;

    // 1. Variablen-Injektion (Suche nach (n))
    while (src[s_idx] != '\0' && t_idx < 1023) {
        if (src[s_idx] == '(' && isdigit((unsigned char)src[s_idx + 1])) {
            int k = s_idx + 1;
            char reg_num[10] = {0};
            int r_idx = 0;
            while (isdigit((unsigned char)src[k]) && r_idx < 9) {
                reg_num[r_idx++] = src[k++];
            }

            if (src[k] == ')') {
                const char* val = mcs_feed_lesen(atoi(reg_num));
                if (val) {
                    for (int v = 0; val[v] != '\0' && t_idx < 1023; v++) {
                        temp[t_idx++] = val[v];
                    }
                }
                s_idx = k + 1;
                continue;
            }
        }
        temp[t_idx++] = src[s_idx++];
    }
    temp[t_idx] = '\0';

    // 2. Trimmen
    char *start_ptr = temp;
    while(*start_ptr && isspace((unsigned char)*start_ptr)) start_ptr++;

    char clean[1024];
    strcpy(clean, start_ptr);

    char *end_ptr = clean + strlen(clean) - 1;
    while(end_ptr >= clean && (isspace((unsigned char)*end_ptr) || *end_ptr == '\n' || *end_ptr == '\r')) {
        *end_ptr = '\0';
        end_ptr--;
    }

    strcpy(dest, clean);
}

// --- DER KERNEL TAKT ---

void mcs_kernel_takt(const char *quellcode) {
    Token *tokens = mcs_lexer_erzeuge(quellcode);
    if (!tokens) return;

    int i = 0;
    int aktionsdraht_aktiv = 1;
    char injected[512];

    while (tokens[i].typ != TOKEN_LEX_EOF) {

        if (tokens[i].typ == TOKEN_TRANS_START) {
            printf("--- MCS KERNEL TAKT START ---\n");
            i++; continue;
        }
        if (tokens[i].typ == TOKEN_TRANS_ENDE) {
            printf("--- MCS KERNEL TAKT ENDE ---\n");
            i++; continue;
        }

        // 2. GATTER-LOGIK (¶ / ¶¶)
        if (tokens[i].typ == TOKEN_PARA_IF || tokens[i].typ == TOKEN_PARA_NOT) {
            int gatter_typ = (tokens[i].typ == TOKEN_PARA_IF) ? 1 : 2;
            char materialisiertes_protein[256] = "";
            int gefunden = 0;

            for (int search = i + 1; search < i + 10 && tokens[search].typ != TOKEN_LEX_EOF; search++) {
                if (tokens[search].wert != NULL && strlen(tokens[search].wert) > 0) {
                    if (tokens[search].typ == TOKEN_AKTION_START) continue;
                    memset(materialisiertes_protein, 0, sizeof(materialisiertes_protein));
                    mcs_inject_vars(materialisiertes_protein, tokens[search].wert);
                    gefunden = 1;
                    break;
                }
            }

            if (gefunden) {
                aktionsdraht_aktiv = mcs_sentiator_evaluiere_boxi(gatter_typ, materialisiertes_protein);
            } else {
                aktionsdraht_aktiv = 0;
            }
            i++; continue;
        }

        // 3. AKTIONSEBENE
        if (aktionsdraht_aktiv) {
            switch (tokens[i].typ) {
                case TOKEN_AKTION_START:
                    printf("» [START]\n");
                    break;
                case TOKEN_AKTION_ENDE:
                    printf("« [ENDE]\n");
                    break;
                case TOKEN_BOXI_SHELL:
                    mcs_inject_vars(injected, tokens[i].wert);
                    mcs_shell_ausfuehren(injected, last_op_result);
                    printf("    [SHELL-OUT]: %s\n", last_op_result);
                    break;
                case TOKEN_BOXI_ALU:
                    mcs_inject_vars(injected, tokens[i].wert);
                    long res = mcs_alu_berechne(injected);
                    sprintf(last_op_result, "%ld", res);
                    printf("    [ALU-RESULT]: %s\n", last_op_result);
                    break;
                case TOKEN_ASSIGN:
                    if (tokens[i+1].typ == TOKEN_REG_ID) {
                        int reg_nr = atoi(tokens[i+1].wert);
                        printf("    [FEED-LOAD]: Port(%d) <== Protein(%s)\n", reg_nr, last_op_result);
                        mcs_feed_laden(reg_nr, last_op_result);
                        i++;
                    }
                    break;
                default: break;
            }
        }
        i++;
    }
    free(tokens);
}
