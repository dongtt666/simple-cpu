#ifndef _TOKEN_H_20251117_
#define _TOKEN_H_20251117_

#define MAX_TOKEN_VALUE_LEN 64
#define MAX_TOKENS 256

/**
 * enum token_type - 令牌类型定义
 *
 * 定义词法分析器能够识别的所有语法单元类型。
 * 每种类型对应源代码中的一个特定语法元素。
 */
typedef enum {
    TOK_EOF,                /**< 文件结束标志，表示源代码已处理完毕 */
    TOK_IDENTIFIER,         /**< 标识符：变量名、函数名等用户定义的名称 */

    /* 算术运算符 */
    TOK_PLUS,               /**< 加法运算符：+ */
    TOK_MINUS,              /**< 减法运算符：- 或负号 */
    TOK_MULTIPLY,           /**< 乘法运算符：* */
    TOK_DIVIDE,             /**< 除法运算符：/ */
    TOK_MODULO,             /**< 取模运算符：% */

    /* 比较运算符 */
    TOK_EQ,                 /**< 等于比较：== */
    TOK_NE,                 /**< 不等于比较：!= */
    TOK_LT,                 /**< 小于比较：< */
    TOK_GT,                 /**< 大于比较：> */
    TOK_LE,                 /**< 小于等于比较：<= */
    TOK_GE,                 /**< 大于等于比较：>= */

    /* 位运算符 */
    TOK_BIT_AND,            /**< 按位与：& */
    TOK_BIT_OR,             /**< 按位或：| */
    TOK_BIT_XOR,            /**< 按位异或：^ */
    TOK_BIT_NOT,            /**< 按位取反：~ */
    TOK_SHL,                /**< 左移运算符：<< */
    TOK_SHR,                /**< 右移运算符：>> */

    /* 逻辑运算符 */
    TOK_LOGICAL_AND,        /**< 逻辑与：&& */
    TOK_LOGICAL_OR,         /**< 逻辑或：|| */
    TOK_LOGICAL_NOT,        /**< 逻辑非：! */

    /* 赋值运算符 */
    TOK_ASSIGN,             /**< 赋值运算符：= */

    /* 分隔符 */
    TOK_SEMICOLON,          /**< 语句结束符：; */
    TOK_COMMA,              /**< 逗号分隔符：, */
    TOK_COLON,              /**< 冒号：: */
    TOK_QUESTION,           /**< 问号：? */

    /* 括号 */
    TOK_LPAREN,             /**< 左圆括号：( */
    TOK_RPAREN,             /**< 右圆括号：) */
    TOK_LBRACE,             /**< 左花括号：{ */
    TOK_RBRACE,             /**< 右花括号：} */
    TOK_LBRACKET,           /**< 左方括号：[ */
    TOK_RBRACKET,           /**< 右方括号：] */

    /* 流程控制关键字 */
    TOK_IF,                 /**< 条件语句关键字：if */
    TOK_ELSE,               /**< 条件语句关键字：else */
    TOK_WHILE,              /**< 循环语句关键字：while */
    TOK_FOR,                /**< 循环语句关键字：for */
    TOK_DO,                 /**< 循环语句关键字：do */
    TOK_BREAK,              /**< 循环控制关键字：break */
    TOK_CONTINUE,           /**< 循环控制关键字：continue */
    TOK_RETURN,             /**< 返回语句关键字：return */

    /* 数据类型关键字 */
    TOK_INT,                /**< 数据类型关键字：int */
    TOK_FLOAT,              /**< 数据类型关键字：float */
    TOK_CHAR,               /**< 数据类型关键字：char */
    TOK_VOID,               /**< 数据类型关键字：void */
    TOK_BOOL,               /**< 数据类型关键字：bool */

    /* 字面量 */
    TOK_NUMBER,             /**< 数字字面量：整数或浮点数的具体数值 */
    TOK_STRING,             /**< 字符串字面量 */
    TOK_CHAR_LITERAL,       /**< 字符字面量 */

    /* 输入输出 */
    TOK_PRINT,              /**< 输出关键字：print */
    TOK_INPUT,              /**< 输入关键字：input */

    /* 特殊令牌 */
    __TOK_MAX,              /**< 令牌类型数量上限，用于数组边界检查 */
    __TOK_ERROR             /**< 错误令牌类型，表示无法识别的语法单元 */
} token_type_t;

/* Token structure */
struct token_st {
    token_type_t type;
    char value[MAX_TOKEN_VALUE_LEN];
    int line;
    int column;
};

const char *token_desc(token_type_t type);

#endif