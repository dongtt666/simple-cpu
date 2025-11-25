#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    AST_PROGRAM, AST_VAR_DECL, AST_ASSIGNMENT, AST_BINARY_OP,
    AST_NUMBER, AST_IDENTIFIER, AST_IF_STMT, AST_WHILE_STMT,
    AST_RETURN_STMT, AST_BLOCK, AST_EXPR_STMT
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char data[100];
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *extra; // 用于第三个子节点（如if语句的else分支）
} ASTNode;

typedef struct {
    Token *tokens;
    int token_count;
    int current_pos;
    Token current_token;
} Parser;

Parser* parser_create(Token *tokens, int token_count);
void parser_advance(Parser *parser);
bool parser_match(Parser *parser, TokenType type);
ASTNode* parse_expression(Parser *parser);
ASTNode* parse_statement(Parser *parser);
ASTNode* parse_program(Parser *parser);
void ast_print(ASTNode *node, int depth);

#endif
