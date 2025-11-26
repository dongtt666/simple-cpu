#ifndef _PARSER_H_20251117_
#define _PARSER_H_20251117_

#include "lexer.h"

#define MAX_OPS_LENGTH 5
#define MAX_STRING_LENGTH 128

/* 简化的AST节点类型 - 专注于代码生成 */
typedef enum {
    /* 声明 */
    NODE_VAR_DECL,      /* 变量声明: int x; */
    NODE_VAR_INIT,      /* 变量初始化: int x = 10; */

    /* 语句 */
    NODE_ASSIGN,        /* 赋值: x = 10; */
    NODE_IF,            /* if语句 */
    NODE_ELSE,          /* else分支 */
    NODE_WHILE,         /* while循环 */
    NODE_FOR,           /* for循环 */
    NODE_DO_WHILE,      /* do-while循环 */
    NODE_RETURN,        /* return语句 */
    NODE_BREAK,         /* break语句 */
    NODE_CONTINUE,      /* continue语句 */
    NODE_BLOCK,         /* 代码块 */
    NODE_EXPR_STMT,     /* 表达式语句 */

    /* 表达式 */
    NODE_BIN_OP,        /* 二元运算: a + b */
    NODE_UNARY_OP,      /* 一元运算: -a, !b */
    NODE_CALL,          /* 函数调用: printf() */
    NODE_ARRAY_ACCESS,  /* 数组访问: a[i] */

    /* 基本元素 */
    NODE_NUMBER,        /* 数字: 123 */
    NODE_FLOAT,         /* 浮点数: 3.14 */
    NODE_STRING,        /* 字符串: "hello" */
    NODE_CHAR,          /* 字符: 'a' */
    NODE_BOOL,          /* 布尔值: true/false */
    NODE_VAR,           /* 变量: x */

    /* 输入输出 */
    NODE_PRINT,         /* 输出语句 */
    NODE_INPUT,         /* 输入语句 */

    /* 程序结构 */
    NODE_PROGRAM,       /* 程序根节点 */
    NODE_FUNCTION       /* 函数定义 */
} ast_node_type_t;

/* 简化的AST节点 */
struct ast_node_st {
    ast_node_type_t type;

    union {
        /* 基本值 */
        struct {
            union {
                int int_val;
                double float_val;
                char char_val;
                // bool bool_val;
            };
            char str_val[MAX_STRING_LENGTH];
        } value;

        /* 标识符 */
        struct {
            char name[MAX_TOKEN_VALUE_LEN];
        } ident;

        /* 控制流 */
        struct {
            struct ast_node_st *cond;
            struct ast_node_st *then_part;
            struct ast_node_st *else_part;
        } if_stmt;

        struct {
            struct ast_node_st *cond;
            struct ast_node_st *body;
        } while_loop;

        struct {
            struct ast_node_st *init;
            struct ast_node_st *cond;
            struct ast_node_st *step;
            struct ast_node_st *body;
        } for_loop;

        struct {
            struct ast_node_st *body;
            struct ast_node_st *cond;
        } do_while_loop;

        /* 代码块 */
        struct {
            struct ast_node_st *stmts;
        } block;

        /* 返回语句 */
        struct {
            struct ast_node_st *expr;
        } return_stmt;

        /* 表达式语句 */
        struct {
            struct ast_node_st *expr;
        } expr_stmt;

        /* 赋值 */
        struct {
            struct ast_node_st *target;
            struct ast_node_st *expr;
        } assign;

        /* 二元运算 */
        struct {
            token_type_t op;
            char op_str[MAX_OPS_LENGTH];
            struct ast_node_st *left;
            struct ast_node_st *right;
        } binary;

        /* 一元运算 */
        struct {
            token_type_t op;
            char op_str[MAX_OPS_LENGTH];
            struct ast_node_st *operand;
        } unary;

        /* 函数调用 */
        struct {
            struct ast_node_st *args;
        } call;

        /* 数组访问 */
        struct {
            struct ast_node_st *array;
            struct ast_node_st *index;
        } array_access;

        /* 函数定义 */
        struct {
            char func_name[MAX_TOKEN_VALUE_LEN];
            struct ast_node_st *params;
            struct ast_node_st *body;
        } function;

        /* 输入输出 */
        struct {
            struct ast_node_st *args;
        } io_stmt;
    } data;

    struct ast_node_st *next;  /* 下一条语句 */
};


/* 语法分析器 */
struct parser_st {
    struct lexer_st *lexer;
    struct token_st *tokens;
    int token_index;
};

/* 函数声明 */
struct parser_st *parser_create(struct lexer_st *lexer);
void parser_destroy(struct parser_st *parser);

struct ast_node_st *ast_node_create(ast_node_type_t type);
void ast_node_destroy(struct ast_node_st *node);
void ast_print_tree(struct ast_node_st *root);

struct ast_node_st *parse_program(struct parser_st *parser);

#endif /* _PARSER_20251117_ */