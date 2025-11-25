#ifndef _LEXER_H_20251117_
#define _LEXER_H_20251117_

typedef enum {
    TOK_EOF, TOK_IDENTIFIER, TOK_NUMBER, TOK_PLUS, TOK_MINUS,
    TOK_MULTIPLY, TOK_DIVIDE, TOK_ASSIGN, TOK_SEMICOLON,
    TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_IF, TOK_ELSE, TOK_WHILE, TOK_RETURN, TOK_INT
} TokenType;

typedef struct {
    TokenType type;
    char value[50];
    int line;
    int column;
} Token;

typedef struct {
    const char *source;
    int position;
    int line;
    int column;
    Token *tokens;
    int token_count;
} Lexer;

Lexer* lexer_create(const char *source);
void lexer_tokenize(Lexer *lexer);
void lexer_print_tokens(Lexer *lexer);

#endif