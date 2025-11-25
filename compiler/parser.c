#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"

Parser* parser_create(Token *tokens, int token_count) {
    Parser *parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->current_pos = 0;
    parser->current_token = tokens[0];
    return parser;
}

void parser_advance(Parser *parser) {
    parser->current_pos++;
    if (parser->current_pos < parser->token_count) {
        parser->current_token = parser->tokens[parser->current_pos];
    }
}

bool parser_match(Parser *parser, TokenType type) {
    if (parser->current_token.type == type) {
        parser_advance(parser);
        return true;
    }
    return false;
}

ASTNode* ast_create_node(ASTNodeType type, const char *data) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    if (data) strcpy(node->data, data);
    node->left = node->right = node->extra = NULL;
    return node;
}

ASTNode* parse_expression(Parser *parser) {
    // 简化版：只处理数字、标识符和基本运算
    if (parser->current_token.type == TOK_NUMBER) {
        ASTNode *node = ast_create_node(AST_NUMBER, parser->current_token.value);
        parser_advance(parser);
        return node;
    }

    if (parser->current_token.type == TOK_IDENTIFIER) {
        ASTNode *node = ast_create_node(AST_IDENTIFIER, parser->current_token.value);
        parser_advance(parser);
        return node;
    }

    if (parser_match(parser, TOK_LPAREN)) {
        ASTNode *expr = parse_expression(parser);
        parser_match(parser, TOK_RPAREN);
        return expr;
    }

    return NULL;
}

ASTNode* parse_statement(Parser *parser) {
    // 变量声明: int x;
    if (parser_match(parser, TOK_INT)) {
        if (parser->current_token.type == TOK_IDENTIFIER) {
            ASTNode *node = ast_create_node(AST_VAR_DECL, parser->current_token.value);
            parser_advance(parser);
            parser_match(parser, TOK_SEMICOLON);
            return node;
        }
    }

    // 赋值语句: x = 10;
    if (parser->current_token.type == TOK_IDENTIFIER) {
        char var_name[50];
        strcpy(var_name, parser->current_token.value);
        parser_advance(parser);

        if (parser_match(parser, TOK_ASSIGN)) {
            ASTNode *value = parse_expression(parser);
            ASTNode *node = ast_create_node(AST_ASSIGNMENT, var_name);
            node->left = value;
            parser_match(parser, TOK_SEMICOLON);
            return node;
        }
    }

    // if 语句
    if (parser_match(parser, TOK_IF)) {
        ASTNode *node = ast_create_node(AST_IF_STMT, "if");
        parser_match(parser, TOK_LPAREN);
        node->left = parse_expression(parser); // 条件
        parser_match(parser, TOK_RPAREN);
        node->right = parse_statement(parser); // then分支
        return node;
    }

    // while 语句
    if (parser_match(parser, TOK_WHILE)) {
        ASTNode *node = ast_create_node(AST_WHILE_STMT, "while");
        parser_match(parser, TOK_LPAREN);
        node->left = parse_expression(parser); // 条件
        parser_match(parser, TOK_RPAREN);
        node->right = parse_statement(parser); // 循环体
        return node;
    }

    // return 语句
    if (parser_match(parser, TOK_RETURN)) {
        ASTNode *node = ast_create_node(AST_RETURN_STMT, "return");
        node->left = parse_expression(parser);
        parser_match(parser, TOK_SEMICOLON);
        return node;
    }

    // 表达式语句
    ASTNode *expr = parse_expression(parser);
    if (expr) {
        parser_match(parser, TOK_SEMICOLON);
        ASTNode *node = ast_create_node(AST_EXPR_STMT, "expr");
        node->left = expr;
        return node;
    }

    return NULL;
}

ASTNode* parse_program(Parser *parser) {
    ASTNode *program = ast_create_node(AST_PROGRAM, "program");
    ASTNode *current = program;

    while (parser->current_token.type != TOK_EOF) {
        ASTNode *stmt = parse_statement(parser);
        if (stmt) {
            // 简单的链表结构连接语句
            current->left = stmt;
            current->right = ast_create_node(AST_PROGRAM, "next");
            current = current->right;
        } else {
            break;
        }
    }

    return program;
}

// ========== AST 打印函数 ==========

const char* ast_type_name(ASTNodeType type) {
    const char *names[] = {
        "PROGRAM", "VAR_DECL", "ASSIGNMENT", "BINARY_OP", "NUMBER",
        "IDENTIFIER", "IF_STMT", "WHILE_STMT", "RETURN_STMT", "BLOCK", "EXPR_STMT"
    };
    return names[type];
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    printf("%s", ast_type_name(node->type));
    if (node->data[0] != '\0') printf(" [%s]", node->data);
    printf("\n");

    ast_print(node->left, depth + 1);
    ast_print(node->right, depth + 1);
    ast_print(node->extra, depth + 1);
}
