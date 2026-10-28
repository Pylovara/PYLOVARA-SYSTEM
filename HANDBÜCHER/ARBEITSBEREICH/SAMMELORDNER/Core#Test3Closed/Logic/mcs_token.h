// mcs_token.h — v3.0 API
#ifndef MCS_TOKEN_H
#define MCS_TOKEN_H
//ausgewechselt #ifndef MCS_LEXER_H
// ausgewechselt #define MCS_LEXER_H

typedef enum {
    TOK_UNKNOWN = 0,
    TOK_COMMENT, TOK_ID, TOK_NUMBER, TOK_LITERAL,
    TOK_TRANS_START, TOK_TRANS_END,
    TOK_ACTION_START, TOK_ACTION_END,
    TOK_PROTEIN_OPEN, TOK_PROTEIN_CLOSE,
    TOK_PROTON_OPEN, TOK_PROTON_CLOSE,
    TOK_SEP,
    TOK_STR_DBL_QUOTE, TOK_STR_SGL_QUOTE,
    TOK_STR_TICK_QUOTE, TOK_BLANKERNER_QUOTE,
    TOK_IF, TOK_ELSE, TOK_PARALLEL_RUN, TOK_PARALLEL_TRANSPORT,
    TOK_WHEN_NOT, TOK_WHEN_GT, TOK_WHEN_LT, TOK_MUST,
    TOK_ON_ERROR, TOK_TRANS_ERROR, TOK_DATA_RESIDUE, TOK_SYNC_TIMER,
    TOK_FEED_OPEN, TOK_FEED_CLOSE,
    TOK_TARGET_REF_PREFIX, TOK_DIRIGENT_PREFIX,
    TOK_WARP, TOK_THETA_ID_PREFIX,
    TOK_OP_MARK, TOK_ARG_MINUS, TOK_ARG_PLUS,
    TOK_ARG_MUL, TOK_ARG_DIV, TOK_ARG_PERCENT,
    TOK_EOF
} mcs_token_type_t;

typedef struct {
    mcs_token_type_t token_type;
    const char* literal;
    int length;
    int line, column;
} mcs_token_t;

typedef struct mcs_lexer mcs_lexer_t;

mcs_lexer_t* mcs_lexer_new(const char* input);
void         mcs_lexer_free(mcs_lexer_t* lex);
mcs_token_t  mcs_lexer_next(mcs_lexer_t* lex);
mcs_token_t mcs_lexer_peek(mcs_lexer_t* lex); // 🔹 NEU

#endif // MCS_TOKEN_H damals  MCS_LEXER_H
