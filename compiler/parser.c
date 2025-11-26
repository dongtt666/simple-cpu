#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/* 节点类型名称 - 使用指定下标确保与ast_node_type_t一一对应 */
static const char *node_names[] = {
    /* 声明 */
    [NODE_VAR_DECL]     = "VAR_DECL",
    [NODE_VAR_INIT]     = "VAR_INIT",

    /* 语句 */
    [NODE_ASSIGN]       = "ASSIGN",
    [NODE_IF]           = "IF",
    [NODE_ELSE]         = "ELSE",
    [NODE_WHILE]        = "WHILE",
    [NODE_FOR]          = "FOR",
    [NODE_DO_WHILE]     = "DO_WHILE",
    [NODE_RETURN]       = "RETURN",
    [NODE_BREAK]        = "BREAK",
    [NODE_CONTINUE]     = "CONTINUE",
    [NODE_BLOCK]        = "BLOCK",
    [NODE_EXPR_STMT]    = "EXPR_STMT",

    /* 表达式 */
    [NODE_BIN_OP]       = "BIN_OP",
    [NODE_UNARY_OP]     = "UNARY_OP",
    [NODE_CALL]         = "CALL",
    [NODE_ARRAY_ACCESS] = "ARRAY_ACCESS",

    /* 字面量 */
    [NODE_NUMBER]       = "NUMBER",
    [NODE_FLOAT]        = "FLOAT",
    [NODE_STRING]       = "STRING",
    [NODE_CHAR]         = "CHAR",
    [NODE_BOOL]         = "BOOL",
    [NODE_VAR]          = "VAR",

    /* 输入输出 */
    [NODE_PRINT]        = "PRINT",
    [NODE_INPUT]        = "INPUT",

    /* 程序结构 */
    [NODE_PROGRAM]      = "PROGRAM",
    [NODE_FUNCTION]     = "FUNCTION"
};

/* 创建语法分析器 */
struct parser_st *parser_create(struct lexer_st *lexer)
{
    struct parser_st *parser = malloc(sizeof(struct parser_st));
    if (!parser) {
        return NULL;
    }

    parser->lexer = lexer;
    parser->token_index = 0;
    return parser;
}

/* 销毁语法分析器 */
void parser_destroy(struct parser_st *parser)
{
    if (parser) free(parser);
}

/* 创建AST节点 */
struct ast_node_st *ast_node_create(ast_node_type_t type)
{
    struct ast_node_st *node = malloc(sizeof(struct ast_node_st));
    if (!node) {
        return NULL;
    }

    node->type = type;
    node->next = NULL;
    memset(&node->data, 0, sizeof(node->data));

    return node;
}

/* 递归销毁AST */
void ast_node_destroy(struct ast_node_st *node)
{
    if (!node) return;

    switch (node->type) {
        case NODE_IF:
            ast_node_destroy(node->data.if_stmt.cond);
            ast_node_destroy(node->data.if_stmt.then_part);
            ast_node_destroy(node->data.if_stmt.else_part);
            break;
        case NODE_WHILE:
            ast_node_destroy(node->data.while_loop.cond);
            ast_node_destroy(node->data.while_loop.body);
            break;
        case NODE_FOR:
            ast_node_destroy(node->data.for_loop.init);
            ast_node_destroy(node->data.for_loop.cond);
            ast_node_destroy(node->data.for_loop.step);
            ast_node_destroy(node->data.for_loop.body);
            break;
        case NODE_DO_WHILE:
            ast_node_destroy(node->data.do_while_loop.body);
            ast_node_destroy(node->data.do_while_loop.cond);
            break;
        case NODE_BLOCK:
            ast_node_destroy(node->data.block.stmts);
            break;
        case NODE_ASSIGN:
            ast_node_destroy(node->data.assign.target);
            ast_node_destroy(node->data.assign.expr);
            break;
        case NODE_BIN_OP:
            ast_node_destroy(node->data.binary.left);
            ast_node_destroy(node->data.binary.right);
            break;
        case NODE_UNARY_OP:
            ast_node_destroy(node->data.unary.operand);
            break;
        case NODE_CALL:
        case NODE_PRINT:
        case NODE_INPUT:
            ast_node_destroy(node->data.call.args);
            break;
        case NODE_ARRAY_ACCESS:
            ast_node_destroy(node->data.array_access.array);
            ast_node_destroy(node->data.array_access.index);
            break;
        case NODE_FUNCTION:
            ast_node_destroy(node->data.function.params);
            ast_node_destroy(node->data.function.body);
            break;
        default:
            break;
    }

    ast_node_destroy(node->next);
    free(node);
}

static inline token_type_t parser_current_type(struct parser_st *parser)
{
    return parser->lexer->tokens[parser->token_index].type;
}

static inline const char *parser_current_value(struct parser_st *parser)
{
    return parser->lexer->tokens[parser->token_index].value;
}

static inline int parser_current_line(struct parser_st *parser)
{
    return parser->lexer->tokens[parser->token_index].line;
}

/* 前进到下一个token */
static inline void parser_next_token(struct parser_st *parser)
{
    parser->token_index++;
}

/* 检查当前token */
static inline int parser_check_token(struct parser_st *parser, token_type_t type)
{
    return (parser->token_index < parser->lexer->token_count &&
        parser->lexer->tokens[parser->token_index].type == type);
}

/* 匹配并前进 */
static inline int parser_match_token(struct parser_st *parser, token_type_t type)
{
    if (parser_check_token(parser, type)) {
        parser_next_token(parser);
        return 1;
    }
    return 0;
}

/* 期待特定token */
static inline int parser_expect_token(struct parser_st *parser, token_type_t type, const char *what)
{
    if (parser_match_token(parser, type)) {
        return 1;
    }

    printf("Syntax Error [Line %d]: Expected %s\n", parser_current_line(parser), what);
    return 0;
}

/* 解析函数声明 */
static struct ast_node_st *parse_statement(struct parser_st *parser);
static struct ast_node_st *parse_block(struct parser_st *parser);
static struct ast_node_st *parse_variable_declaration(struct parser_st *parser);
static struct ast_node_st *parse_if_statement(struct parser_st *parser);
static struct ast_node_st *parse_while_statement(struct parser_st *parser);
static struct ast_node_st *parse_for_statement(struct parser_st *parser);
static struct ast_node_st *parse_return_statement(struct parser_st *parser);
static struct ast_node_st *parse_break_statement(struct parser_st *parser);
static struct ast_node_st *parse_continue_statement(struct parser_st *parser);
static struct ast_node_st *parse_print_statement(struct parser_st *parser);
static struct ast_node_st *parse_expression_statement(struct parser_st *parser);

static struct ast_node_st *parse_expression(struct parser_st *parser);
static struct ast_node_st *parse_assignment(struct parser_st *parser);
static struct ast_node_st *parse_primary(struct parser_st *parser);
static struct ast_node_st *parse_unary(struct parser_st *parser);
static struct ast_node_st *parse_multiplicative(struct parser_st *parser);
static struct ast_node_st *parse_additive(struct parser_st *parser);
static struct ast_node_st *parse_relational(struct parser_st *parser);
static struct ast_node_st *parse_equality(struct parser_st *parser);


/* 解析程序 */
struct ast_node_st *parse_program(struct parser_st *parser)
{
    struct ast_node_st *program = ast_node_create(NODE_PROGRAM);
    struct ast_node_st *current = program;

    if (!program) {
        return NULL;
    }

    while (!parser_check_token(parser, TOK_EOF)) {
        struct ast_node_st *stmt = parse_statement(parser);
        if (!stmt) {
            if (!parser_check_token(parser, TOK_EOF)) {
                printf("[Line %d]: Unable to parse statement\n", parser_current_line(parser));
            }
            break;
        }

        current->next = stmt;
        current = stmt;
    }

    return program;
}

/* 解析语句 */
static struct ast_node_st *parse_statement(struct parser_st *parser)
{
    struct ast_node_st *stmt = NULL;

    /* 尝试各种语句类型 */
    if ((stmt = parse_block(parser)) != NULL) return stmt;
    if ((stmt = parse_variable_declaration(parser)) != NULL) return stmt;
    if ((stmt = parse_if_statement(parser)) != NULL) return stmt;
    if ((stmt = parse_while_statement(parser)) != NULL) return stmt;
    if ((stmt = parse_for_statement(parser)) != NULL) return stmt;
    if ((stmt = parse_return_statement(parser)) != NULL) return stmt;
    if ((stmt = parse_break_statement(parser)) != NULL) return stmt;
    if ((stmt = parse_continue_statement(parser)) != NULL) return stmt;
    if ((stmt = parse_print_statement(parser)) != NULL) return stmt;

    return parse_expression_statement(parser);
}

/* 解析基本表达式 */
static struct ast_node_st *parse_primary(struct parser_st *parser)
{
    struct ast_node_st *node = NULL;
    char token_value[MAX_TOKEN_VALUE_LEN];
    strcpy(token_value, parser_current_value(parser));
    int line = parser_current_line(parser);

    /* 数字字面量 */
    if (parser_match_token(parser, TOK_NUMBER)) {
        node = ast_node_create(NODE_NUMBER);
        node->data.value.int_val = atoi(token_value);
        return node;
    }

    /* 字符串字面量 */
    if (parser_match_token(parser, TOK_STRING)) {
        node = ast_node_create(NODE_STRING);
        strcpy(node->data.value.str_val, token_value);
        return node;
    }

    /* 标识符 - 可能是变量、函数调用或数组访问 */
    if (parser_match_token(parser, TOK_IDENTIFIER)) {
        /* 函数调用 */
        if (parser_match_token(parser, TOK_LPAREN)) {
            node = ast_node_create(NODE_CALL);
            strcpy(node->data.ident.name, token_value);
            /* 简化：不解析参数，直接跳到右括号 */
            while (!parser_check_token(parser, TOK_RPAREN) && !parser_check_token(parser, TOK_EOF)) {
                parser_next_token(parser);
            }
            parser_expect_token(parser, TOK_RPAREN, "')'");
            return node;
        }
        /* 数组访问 */
        /* 普通变量 */
        node = ast_node_create(NODE_VAR);
        strcpy(node->data.ident.name, token_value);
        return node;
    }

    /* 括号表达式 */
    if (parser_match_token(parser, TOK_LPAREN)) {
        node = parse_primary(parser);  /* 简化：直接递归解析 */
        parser_expect_token(parser, TOK_RPAREN, "')'");
        return node;
    }

    printf("Syntax Error [Line %d]: Expected expression\n", line);
    return NULL;
}

/* 解析一元表达式 */
static struct ast_node_st *parse_unary(struct parser_st *parser)
{
    /* 检查一元操作符 */
    if (parser_match_token(parser, TOK_MINUS) || parser_match_token(parser, TOK_PLUS) ||
        parser_match_token(parser, TOK_LOGICAL_NOT) || parser_match_token(parser, TOK_BIT_NOT)) {

        struct ast_node_st *node = ast_node_create(NODE_UNARY_OP);
        node->data.unary.op = parser_current_type(parser);
        strcpy(node->data.unary.op_str, parser_current_value(parser));
        node->data.unary.operand = parse_unary(parser);
        return node;
    }

    return parse_primary(parser);
}

/* 解析乘法类表达式 */
static struct ast_node_st *parse_multiplicative(struct parser_st *parser)
{
    struct ast_node_st *left = parse_unary(parser);

    while (parser_check_token(parser, TOK_MULTIPLY) || parser_check_token(parser, TOK_DIVIDE) ||
           parser_check_token(parser, TOK_MODULO)) {
        struct ast_node_st *node = ast_node_create(NODE_BIN_OP);
        node->data.binary.op = parser_current_type(parser);;
        strcpy(node->data.binary.op_str, parser_current_value(parser));
        parser_next_token(parser);

        node->data.binary.left = left;
        node->data.binary.right = parse_unary(parser);
        left = node;
    }

    return left;
}

/* 解析加法类表达式 */
static struct ast_node_st *parse_additive(struct parser_st *parser)
{
    struct ast_node_st *left = parse_multiplicative(parser);

    while (parser_check_token(parser, TOK_PLUS) || parser_check_token(parser, TOK_MINUS)) {
        struct ast_node_st *node = ast_node_create(NODE_BIN_OP);
        node->data.binary.op = parser_current_type(parser);;
        strcpy(node->data.binary.op_str, parser_current_value(parser));
        parser_next_token(parser);

        node->data.binary.left = left;
        node->data.binary.right = parse_multiplicative(parser);
        left = node;
    }

    return left;
}

/* 解析比较表达式 */
static struct ast_node_st *parse_relational(struct parser_st *parser)
{
    struct ast_node_st *left = parse_additive(parser);

    while (parser_check_token(parser, TOK_LT) || parser_check_token(parser, TOK_GT) ||
           parser_check_token(parser, TOK_LE) || parser_check_token(parser, TOK_GE)) {
        struct ast_node_st *node = ast_node_create(NODE_BIN_OP);
        node->data.binary.op = parser_current_type(parser);;
        strcpy(node->data.binary.op_str, parser_current_value(parser));
        parser_next_token(parser);

        node->data.binary.left = left;
        node->data.binary.right = parse_additive(parser);
        left = node;
    }

    return left;
}

/* 解析相等性表达式 */
static struct ast_node_st *parse_equality(struct parser_st *parser)
{
    struct ast_node_st *left = parse_relational(parser);

    while (parser_check_token(parser, TOK_EQ) || parser_check_token(parser, TOK_NE)) {
        struct ast_node_st *node = ast_node_create(NODE_BIN_OP);
        node->data.binary.op = parser_current_type(parser);;
        strcpy(node->data.binary.op_str, parser_current_value(parser));
        parser_next_token(parser);

        node->data.binary.left = left;
        node->data.binary.right = parse_relational(parser);
        left = node;
    }

    return left;
}

/* 解析赋值表达式 */
static struct ast_node_st *parse_assignment(struct parser_st *parser)
{
    struct ast_node_st *left = parse_equality(parser);

    if (parser_match_token(parser, TOK_ASSIGN)) {
        struct ast_node_st *node = ast_node_create(NODE_ASSIGN);
        node->data.assign.target = left;
        node->data.assign.expr = parse_assignment(parser);
        return node;
    }

    return left;
}

/* 解析表达式 */
static struct ast_node_st *parse_expression(struct parser_st *parser)
{
    return parse_assignment(parser);
}

static struct ast_node_st *parse_expression_statement(struct parser_st *parser)
{
    struct ast_node_st *node = ast_node_create(NODE_EXPR_STMT);
    node->data.expr_stmt.expr = parse_expression(parser);
    parser_expect_token(parser, TOK_SEMICOLON, "';'");
    return node;
}

/* 解析变量声明 */
static struct ast_node_st *parse_variable_declaration(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_INT) && !parser_match_token(parser, TOK_FLOAT) &&
        !parser_match_token(parser, TOK_CHAR) && !parser_match_token(parser, TOK_BOOL)) {
        return NULL;
    }

    int line = parser_current_line(parser);
    if (!parser_check_token(parser, TOK_IDENTIFIER)) {
        printf("[Line %d]: Expected variable name\n", line);
        return NULL;
    }

    char var_name[MAX_TOKEN_VALUE_LEN];
    strcpy(var_name, parser_current_value(parser));
    parser_next_token(parser); /* 消费变量名 */

    /* 变量声明: int x; */
    if (parser_match_token(parser, TOK_SEMICOLON)) {
        struct ast_node_st *node = ast_node_create(NODE_VAR_DECL);
        strcpy(node->data.ident.name, var_name);
        return node;
    }

    /* 变量初始化: int x = 10; */
    if (parser_match_token(parser, TOK_ASSIGN)) {
        struct ast_node_st *node = ast_node_create(NODE_VAR_INIT);
        strcpy(node->data.ident.name, var_name);
        node->data.assign.expr = parse_expression(parser);
        parser_expect_token(parser, TOK_SEMICOLON, "';'");
        return node;
    }

    printf("[Line %d]: Expected ';' or '='\n", line);
    return NULL;
}

/* 代码块解析 - 解析 { statement1; statement2; ... } */
static struct ast_node_st *parse_block(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_LBRACE)) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_BLOCK);
    struct ast_node_st *current = NULL;

    /* 解析块内的所有语句 */
    while (!parser_check_token(parser, TOK_RBRACE) && !parser_check_token(parser, TOK_EOF)) {
        struct ast_node_st *stmt = parse_statement(parser);
        if (stmt) {
            if (!node->data.block.stmts) {
                node->data.block.stmts = stmt;
                current = stmt;
            } else {
                current->next = stmt;
                current = stmt;
            }
        } else {
            /* 解析失败，跳过当前token */
            parser_next_token(parser);
        }
    }

    if (!parser_expect_token(parser, TOK_RBRACE, "'}'")) {
        return node;
    }

    return node;
}

/* 解析if语句 */
static struct ast_node_st *parse_if_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_IF)) {
        return NULL;
    }

    if (!parser_expect_token(parser, TOK_LPAREN, "'('")) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_IF);
    node->data.if_stmt.cond = parse_expression(parser);

    if (!parser_expect_token(parser, TOK_RPAREN, "')'")) {
        return node;
    }

    node->data.if_stmt.then_part = parse_statement(parser); /* 简化：用表达式代替语句 */

    if (parser_match_token(parser, TOK_ELSE)) {
        node->data.if_stmt.else_part = parse_statement(parser);
    }

    return node;
}

/* 解析while语句 */
static struct ast_node_st *parse_while_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_WHILE)) {
        return NULL;
    }

    if (!parser_expect_token(parser, TOK_LPAREN, "'('")) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_WHILE);
    node->data.while_loop.cond = parse_expression(parser);

    if (!parser_expect_token(parser, TOK_RPAREN, "')'")) {
        return node;
    }

    node->data.while_loop.body = parse_statement(parser);
    return node;
}

/* 解析for语句 */
static struct ast_node_st *parse_for_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_FOR)) {
        return NULL;
    }

    if (!parser_expect_token(parser, TOK_LPAREN, "'('")) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_FOR);

    /* 初始化部分 */
    if (!parser_check_token(parser, TOK_SEMICOLON)) {
        node->data.for_loop.init = parse_expression(parser);
    }
    parser_expect_token(parser, TOK_SEMICOLON, "';'");

    /* 条件部分 */
    if (!parser_check_token(parser, TOK_SEMICOLON)) {
        node->data.for_loop.cond = parse_expression(parser);
    }
    parser_expect_token(parser, TOK_SEMICOLON, "';'");

    /* 步进部分 */
    if (!parser_check_token(parser, TOK_RPAREN)) {
        node->data.for_loop.step = parse_expression(parser);
    }
    parser_expect_token(parser, TOK_RPAREN, "')'");

    node->data.for_loop.body = parse_statement(parser);
    return node;
}

/* 解析return语句 */
static struct ast_node_st *parse_return_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_RETURN)) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_RETURN);

    if (!parser_check_token(parser, TOK_SEMICOLON)) {
        node->data.return_stmt.expr = parse_expression(parser);
    }

    parser_expect_token(parser, TOK_SEMICOLON, "';'");
    return node;
}

/* 解析break语句 */
static struct ast_node_st *parse_break_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_BREAK)) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_BREAK);
    parser_expect_token(parser, TOK_SEMICOLON, "';'");
    return node;
}

/* 解析continue语句 */
static struct ast_node_st *parse_continue_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_CONTINUE)) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_CONTINUE);
    parser_expect_token(parser, TOK_SEMICOLON, "';'");
    return node;
}

/* 解析print语句 */
static struct ast_node_st *parse_print_statement(struct parser_st *parser)
{
    if (!parser_match_token(parser, TOK_PRINT)) {
        return NULL;
    }

    struct ast_node_st *node = ast_node_create(NODE_PRINT);

    if (parser_match_token(parser, TOK_LPAREN)) {
        node->data.call.args = parse_expression(parser);
        parser_expect_token(parser, TOK_RPAREN, "')'");
    }

    parser_expect_token(parser, TOK_SEMICOLON, "';'");
    return node;
}

/* 打印AST节点信息 */
static void ast_print_node(struct ast_node_st *node, int depth)
{
    if (!node) return;

    /* 缩进 */
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    /* 打印节点基本信息 */
    printf("%s", node_names[node->type]);

    /* 打印节点特定信息 */
    switch (node->type) {
        case NODE_NUMBER:
            printf(" Value: %d", node->data.value.int_val);
            break;
        case NODE_STRING:
            printf(" Value: \"%s\"", node->data.value.str_val);
            break;
        case NODE_VAR:
        case NODE_VAR_DECL:
        case NODE_VAR_INIT:
            printf(" Name: %s", node->data.ident.name);
            break;
        case NODE_BIN_OP:
        case NODE_UNARY_OP:
            printf(" Operator: %s", node->data.binary.op_str);
            break;
        case NODE_CALL:
        case NODE_PRINT:
            printf(" Function: %s", node->data.ident.name);
            break;
        default:
            break;
    }
    printf("\n");

    /* 递归打印子节点 */
    switch (node->type) {
        case NODE_BLOCK:
            ast_print_node(node->data.block.stmts, depth + 1);
            break;
        case NODE_IF:
            ast_print_node(node->data.if_stmt.cond, depth + 1);
            ast_print_node(node->data.if_stmt.then_part, depth + 1);
            if (node->data.if_stmt.else_part)
                ast_print_node(node->data.if_stmt.else_part, depth + 1);
            break;
        case NODE_WHILE:
            ast_print_node(node->data.while_loop.cond, depth + 1);
            ast_print_node(node->data.while_loop.body, depth + 1);
            break;
        case NODE_FOR:
            ast_print_node(node->data.for_loop.init, depth + 1);
            ast_print_node(node->data.for_loop.cond, depth + 1);
            ast_print_node(node->data.for_loop.step, depth + 1);
            ast_print_node(node->data.for_loop.body, depth + 1);
            break;
        case NODE_ASSIGN:
            ast_print_node(node->data.assign.target, depth + 1);
            ast_print_node(node->data.assign.expr, depth + 1);
            break;
        case NODE_BIN_OP:
            ast_print_node(node->data.binary.left, depth + 1);
            ast_print_node(node->data.binary.right, depth + 1);
            break;
        case NODE_UNARY_OP:
            ast_print_node(node->data.unary.operand, depth + 1);
            break;
        case NODE_CALL:
        case NODE_PRINT:
            ast_print_node(node->data.call.args, depth + 1);
            break;
        case NODE_ARRAY_ACCESS:
            ast_print_node(node->data.array_access.array, depth + 1);
            ast_print_node(node->data.array_access.index, depth + 1);
            break;
        case NODE_VAR_INIT:
            ast_print_node(node->data.assign.expr, depth + 1);
            break;
        case NODE_RETURN:
            ast_print_node(node->data.return_stmt.expr, depth + 1);
            break;
        case NODE_EXPR_STMT:
            ast_print_node(node->data.expr_stmt.expr, depth + 1);
            break;
        default:
            break;
    }

    /* 打印链表中的下一个语句 */
    if (node->next) {
        ast_print_node(node->next, depth);
    }
}

/* 打印用于代码生成的AST信息 */
void ast_print_tree(struct ast_node_st *root)
{
    if (root) {
        printf("=== Syntax Analysis Result (AST) ===\n");
        ast_print_node(root, 0);
    }
}
