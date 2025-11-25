#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"


Lexer* lexer_create(const char *source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
    lexer->tokens = malloc(100 * sizeof(Token));
    return lexer;
}

void lexer_add_token(Lexer *lexer, TokenType type, const char *value) {
    Token *token = &lexer->tokens[lexer->token_count++];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    if (value) strcpy(token->value, value);
}

void lexer_tokenize(Lexer *lexer) {
    while (lexer->source[lexer->position]) {
        char c = lexer->source[lexer->position];

        if (isspace(c)) {
            if (c == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }

        if (isalpha(c)) {
            // 标识符或关键字
            char buffer[50] = {0};
            int i = 0;
            while (isalnum(lexer->source[lexer->position])) {
                buffer[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            buffer[i] = '\0';

            if (strcmp(buffer, "if") == 0) lexer_add_token(lexer, TOK_IF, "if");
            else if (strcmp(buffer, "else") == 0) lexer_add_token(lexer, TOK_ELSE, "else");
            else if (strcmp(buffer, "while") == 0) lexer_add_token(lexer, TOK_WHILE, "while");
            else if (strcmp(buffer, "return") == 0) lexer_add_token(lexer, TOK_RETURN, "return");
            else if (strcmp(buffer, "int") == 0) lexer_add_token(lexer, TOK_INT, "int");
            else lexer_add_token(lexer, TOK_IDENTIFIER, buffer);
            continue;
        }

        if (isdigit(c)) {
            // 数字
            char buffer[50] = {0};
            int i = 0;
            while (isdigit(lexer->source[lexer->position])) {
                buffer[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            buffer[i] = '\0';
            lexer_add_token(lexer, TOK_NUMBER, buffer);
            continue;
        }

        // 单字符token
        switch (c) {
            case '+': lexer_add_token(lexer, TOK_PLUS, "+"); break;
            case '-': lexer_add_token(lexer, TOK_MINUS, "-"); break;
            case '*': lexer_add_token(lexer, TOK_MULTIPLY, "*"); break;
            case '/': lexer_add_token(lexer, TOK_DIVIDE, "/"); break;
            case '=': lexer_add_token(lexer, TOK_ASSIGN, "="); break;
            case ';': lexer_add_token(lexer, TOK_SEMICOLON, ";"); break;
            case '(': lexer_add_token(lexer, TOK_LPAREN, "("); break;
            case ')': lexer_add_token(lexer, TOK_RPAREN, ")"); break;
            case '{': lexer_add_token(lexer, TOK_LBRACE, "{"); break;
            case '}': lexer_add_token(lexer, TOK_RBRACE, "}"); break;
            default:
                printf("未知字符: %c\n", c);
                lexer->position++;
                lexer->column++;
                continue;
        }
        lexer->position++;
        lexer->column++;
    }
    lexer_add_token(lexer, TOK_EOF, "EOF");
}

void lexer_print_tokens(Lexer *lexer) {
    printf("=== 词法分析结果 ===\n");
    for (int i = 0; i < lexer->token_count; i++) {
        const char *type_names[] = {
            "EOF", "IDENTIFIER", "NUMBER", "PLUS", "MINUS", "MULTIPLY",
            "DIVIDE", "ASSIGN", "SEMICOLON", "LPAREN", "RPAREN",
            "LBRACE", "RBRACE", "IF", "ELSE", "WHILE", "RETURN", "INT"
        };
        printf("行%2d, 列%2d: %-12s '%s'\n",
               lexer->tokens[i].line, lexer->tokens[i].column,
               type_names[lexer->tokens[i].type], lexer->tokens[i].value);
    }
    printf("共 %d 个token\n\n", lexer->token_count);
}
