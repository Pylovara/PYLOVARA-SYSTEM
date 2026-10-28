// Lab/test_lexer.c — v2.9-konform, UTF-8-sicher, FEHLERFREI
#include "mcs_token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* token_name(mcs_token_type_t t) {
    switch (t) {
        case TOK_TRANS_START: return "TOK_TRANS_START";
        case TOK_TRANS_END: return "TOK_TRANS_END";
        case TOK_ACTION_START: return "TOK_ACTION_START";
        case TOK_ACTION_END: return "TOK_ACTION_END";
        case TOK_OP_MARK: return "TOK_OP_MARK";
        case TOK_FEED_OPEN: return "TOK_FEED_OPEN";
        case TOK_FEED_CLOSE: return "TOK_FEED_CLOSE";
        case TOK_BLANKERNER_QUOTE: return "TOK_BLANKERNER_QUOTE";
        case TOK_STR_SGL_QUOTE: return "TOK_STR_SGL_QUOTE";
        case TOK_ID: return "TOK_ID";
        case TOK_NUMBER: return "TOK_NUMBER";
        case TOK_COMMENT: return "TOK_COMMENT";
        case TOK_EOF: return "TOK_EOF";
        default: return "OTHER";
    }
}

int main() {
    // Lese Lab/test.mcs
    const char* filename = "Lab/test.mcs";
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "[ERROR] Kann '%s' nicht öffnen.\n", filename);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buf = malloc(size + 1);
    if (!buf) {
        perror("malloc");
        fclose(f);
        return 1;
    }
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);

    printf("Tokens in %s:\n", filename);
    mcs_lexer_t* lex = mcs_lexer_new(buf);  // ← deklariert VOR Schleife
    if (!lex) {
        fprintf(stderr, "[ERROR] Lexer-Init fehlgeschlagen.\n");
        free(buf);
        return 1;
    }

    mcs_token_t tok;  // ← deklariert VOR Schleife
    while ((tok = mcs_lexer_next(lex)).token_type != TOK_EOF) {
        // Sicherer Text-Extrakt (kein Absturz bei \0 im UTF-8)
        char* text = malloc(tok.length + 1);
        if (!text) break;
        memcpy(text, tok.literal, tok.length);
        text[tok.length] = '\0';

        // Drucke lesbare Bytes
        printf("  [%s] '", token_name(tok.token_type));
        for (int i = 0; i < tok.length; i++) {
            unsigned char c = (unsigned char)text[i];
            if (c >= 32 && c < 127) {
                putchar(c);
            } else {
                printf("\\x%02X", c);
            }
        }
        printf("' (line %d, col %d)\n", tok.line, tok.column);

        free(text);
    }

    mcs_lexer_free(lex);
    free(buf);
    printf("=== Lexing abgeschlossen ===\n");

    // Füge am Ende von main() hinzu:
    printf("\n--- Peek-Test ---\n");
    mcs_lexer_t* lex2 = mcs_lexer_new("¶ ¶¶");
    mcs_token_t t1 = mcs_lexer_next(lex2);     // ¶
    mcs_token_t peek = mcs_lexer_peek(lex2);   // Vorausschau: ¶¶
    mcs_token_t t2 = mcs_lexer_next(lex2);     // ¶¶
    printf("next: %s, peek: %s, next: %s\n",
           token_name(t1.token_type),
           token_name(peek.token_type),
           token_name(t2.token_type));
    mcs_lexer_free(lex2);

    return 0;
}
