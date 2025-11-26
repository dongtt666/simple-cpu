#ifndef _LEXER_H_20251117_
#define _LEXER_H_20251117_

#include "token.h"

/* Lexer structure */
struct lexer_st {
    const char *source;
    int position;
    int line;
    int column;
    int token_count;
    struct token_st *tokens;
};

/* Function declarations */
struct lexer_st *lexer_create(const char *source);
int lexer_tokenize(struct lexer_st *lexer);
void lexer_destroy(struct lexer_st *lexer);
void lexer_print_tokens(const struct lexer_st *lexer);

#endif