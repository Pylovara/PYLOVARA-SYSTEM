// /Pylovara/MCSKernel/include/mcs_lexer.h
#ifndef MCS_LEXER_H
#define MCS_LEXER_H

typedef enum {
    TOKEN_TRANS_START,    // [
    TOKEN_TRANS_ENDE,     // ]
    TOKEN_ARG_TRIGGER,
    TOKEN_ARG_TIME,       // Entspricht T
    TOKEN_PARA_IF,        // ¶
    TOKEN_PARA_NOT,       // ¶¶ (Neu hinzugefügt für Negation/Gatter)
    TOKEN_PARA_ELSE,
    TOKEN_REINHEIT,
    TOKEN_NEGATION,
    TOKEN_BOXICHECK,
    TOKEN_ALARM,
    TOKEN_AKTION_START,   // »
    TOKEN_AKTION_ENDE,    // «
    TOKEN_BOXI_ALU,
    TOKEN_BOXI_BLANK,
    TOKEN_BOXI_SHELL,     // { ... }
    TOKEN_ASSIGN,
    TOKEN_REG_ID,
    TOKEN_FOR_PROGRESS,
    TOKEN_LEX_EOF
} TokenType;

typedef struct {
    TokenType typ;
    char wert[256];       // Vergrößert auf 256 für längere Shell-Befehle
    int reg_index;        // UNVERZICHTBAR: Speichert die (3) aus [(3)]
} Token;

Token* mcs_lexer_erzeuge(const char *quellcode);

#endif
