#include "token.h"

/**
 * s_token_descs - 令牌类型描述字符串数组
 *
 * 将令牌类型枚举值映射到人类可读的字符串描述。
 * 用于调试输出、错误报告和日志记录。
 *
 * 注意：数组顺序必须与 token_type_t 枚举定义完全一致。
 */
static const char * const s_token_descs[] = {
    [TOK_EOF]               = "EOF",
    [TOK_IDENTIFIER]        = "IDENTIFIER",

    /* 算术运算符 */
    [TOK_PLUS]              = "PLUS",
    [TOK_MINUS]             = "MINUS",
    [TOK_MULTIPLY]          = "MULTIPLY",
    [TOK_DIVIDE]            = "DIVIDE",
    [TOK_MODULO]            = "MODULO",

    /* 比较运算符 */
    [TOK_EQ]                = "EQUAL",
    [TOK_NE]                = "NOT_EQUAL",
    [TOK_LT]                = "LESS_THAN",
    [TOK_GT]                = "GREATER_THAN",
    [TOK_LE]                = "LESS_EQUAL",
    [TOK_GE]                = "GREATER_EQUAL",

    /* 位运算符 */
    [TOK_BIT_AND]           = "BITWISE_AND",
    [TOK_BIT_OR]            = "BITWISE_OR",
    [TOK_BIT_XOR]           = "BITWISE_XOR",
    [TOK_BIT_NOT]           = "BITWISE_NOT",
    [TOK_SHL]               = "SHIFT_LEFT",
    [TOK_SHR]               = "SHIFT_RIGHT",

    /* 逻辑运算符 */
    [TOK_LOGICAL_AND]       = "LOGICAL_AND",
    [TOK_LOGICAL_OR]        = "LOGICAL_OR",
    [TOK_LOGICAL_NOT]       = "LOGICAL_NOT",

    /* 赋值运算符 */
    [TOK_ASSIGN]            = "ASSIGN",

    /* 分隔符 */
    [TOK_SEMICOLON]         = "SEMICOLON",
    [TOK_COMMA]             = "COMMA",
    [TOK_COLON]             = "COLON",
    [TOK_QUESTION]          = "QUESTION",

    /* 括号 */
    [TOK_LPAREN]            = "LEFT_PAREN",
    [TOK_RPAREN]            = "RIGHT_PAREN",
    [TOK_LBRACE]            = "LEFT_BRACE",
    [TOK_RBRACE]            = "RIGHT_BRACE",
    [TOK_LBRACKET]          = "LEFT_BRACKET",
    [TOK_RBRACKET]          = "RIGHT_BRACKET",

    /* 关键字 */
    [TOK_IF]                = "IF",
    [TOK_ELSE]              = "ELSE",
    [TOK_WHILE]             = "WHILE",
    [TOK_FOR]               = "FOR",
    [TOK_DO]                = "DO",
    [TOK_BREAK]             = "BREAK",
    [TOK_CONTINUE]          = "CONTINUE",
    [TOK_RETURN]            = "RETURN",

    /* 数据类型关键字 */
    [TOK_INT]               = "INT",
    [TOK_FLOAT]             = "FLOAT",
    [TOK_CHAR]              = "CHAR",
    [TOK_VOID]              = "VOID",
    [TOK_BOOL]              = "BOOL",

    /* 字面量 */
    [TOK_NUMBER]            = "NUMBER",
    [TOK_STRING]            = "STRING",
    [TOK_CHAR_LITERAL]      = "CHAR_LITERAL",

    /* 输入输出 */
    [TOK_PRINT]             = "PRINT",
    [TOK_INPUT]             = "INPUT",

    /* 特殊令牌 */
    [__TOK_MAX]         = "__TOKEN_MAX",
    [__TOK_ERROR]       = "__TOKEN_ERROR",
};

const char *token_desc(token_type_t type)
{
    if (type >= TOK_EOF && type < __TOK_MAX)
        return s_token_descs[type];
    return s_token_descs[__TOK_ERROR];
}
