好的，我们来详细介绍一下 Flex 和 Bison 的使用，并提供一个完整的例子。

### Flex & Bison 简介

Flex 和 Bison 是用于构建**语言处理程序**（如编译器、解释器）的经典工具组合。它们是 Lex 和 Yacc 的 GNU 版本。

*   **Flex (Fast Lexical Analyzer Generator)：**
    *   **用途**：**词法分析**。
    *   **输入**：一个 `.l` 文件，其中定义了**词法规则**（正则表达式）。
    *   **输出**：一个 C 源文件（如 `lex.yy.c`），这个文件包含一个 `yylex()` 函数。该函数读取输入流，并根据规则将其分割成一个个的 **Token（词法单元）**。

*   **Bison (GNU Parser Generator)：**
    *   **用途**：**语法分析**。
    *   **输入**：一个 `.y` 文件，其中定义了**语法规则**（BNF范式）。
    *   **输出**：一个 C 源文件（如 `y.tab.c`），这个文件包含一个 `yyparse()` 函数。该函数调用 `yylex()` 来获取 Token，并尝试根据语法规则构建**抽象语法树（AST）**，判断输入是否符合语法。

---

### 工作流程与交互

它们协同工作的核心流程，以及它们与你代码的交互关系，可以通过下图来清晰地展示：

```mermaid
sequenceDiagram
    participant User
    participant yyparse (Bison)
    participant yylex (Flex)
    participant User Code

    Note over yyparse, User Code: Bison 生成的解析器
    Note over yylex: Flex 生成的扫描器

    User->>+yyparse: 输入字符串 (e.g., "a = 3 + 5;")
    loop 解析过程
        yyparse->>+yylex: 调用 yylex()
        Note over yylex: 读取字符流，<br>匹配正则规则
        yylex-->>-yyparse: 返回一个Token (e.g., ID, NUMBER) <br>& 关联值 (e.g., "a", 3)
        Note over yyparse: 根据返回的Token<br>按BNF语法规则进行归约
    end
    Note over yyparse: 解析成功/失败

    alt 解析成功 (归约到起始符号)
        yyparse->>+User Code: 执行对应规则关联的动作
        User Code-->>-User Code: (e.g., 构建AST节点，<br>执行计算，打印信息)
    else 解析失败 (遇到语法错误)
        yyparse->>User Code: 调用 yyerror()
    end
    yyparse-->>-User: 返回解析结果
```

---

### 文件结构与基本语法

#### 1. Flex 文件 (`lexer.l`) 结构

```
%{
    /* C 代码和头文件声明 */
    #include "y.tab.h" // 包含 Bison 生成的 Token 定义
%}

/* 正则表达式定义 */
%%
    /* 词法规则部分 */
{正则表达式}    { 动作（C代码） }
%%
    /* 用户自定义的 C 辅助函数 (可选) */
```

#### 2. Bison 文件 (`parser.y`) 结构

```
%{
    /* C 代码和头文件声明 */
    #include <stdio.h>
    /* 函数声明 */
    int yylex(void);
    void yyerror(const char *s);
%}

/* Bison 声明部分 */
%token [Token 定义，如 NUMBER ID]
%start [起始符号]

%%
    /* 语法规则部分 */
[非终结符] : [产生式] { 动作（C代码） }
          ;
%%
    /* 用户自定义的 C 辅助函数 (如 yyerror) */
```

---

### 完整示例：一个简单的计算器

这个计算器支持 `+`, `-`, `*`, `/` 和括号。

#### 1. Bison 语法文件 (`calculator.y`)

```c
%{
    #include <stdio.h>
    #include <stdlib.h>

    // 声明 Flex 提供的函数
    int yylex(void);
    void yyerror(const char *s);

    // 在 Bison 动作中使用的变量，用于存储数值
    int result;
%}

/* 定义词法分析器返回的 TOKEN */
%token NUMBER
%token ADD SUB MUL DIV LPAREN RPAREN NEWLINE

/* 定义运算符的结合性和优先级 */
%left ADD SUB
%left MUL DIV
%left UMINUS // 单目负号的优先级

/* 告诉 Bison 整个语法的起始符号是 line */
%start line

%%

/* 语法规则 */
line
    : /* 空规则 */
    | expression NEWLINE  {
        printf(" = %d\n", $1);
        result = $1;
    }
    ;

expression
    : NUMBER              { $$ = $1; }
    | expression ADD expression { $$ = $1 + $3; }
    | expression SUB expression { $$ = $1 - $3; }
    | expression MUL expression { $$ = $1 * $3; }
    | expression DIV expression {
            if ($3 == 0) {
                yyerror("Division by zero!");
                exit(1);
            }
            $$ = $1 / $3;
        }
    | LPAREN expression RPAREN { $$ = $2; }
    | SUB expression %prec UMINUS { $$ = -$2; } // 单目负号
    ;

%%

/* 自定义函数 */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Simple Calculator. Enter expressions (e.g., 2 + 3 * (4 - 1)):\n");
    // 开始解析
    yyparse();
    return 0;
}
```

#### 2. Flex 词法文件 (`calculator.l`)

```c
%{
    // 包含 Bison 生成的头文件，里面定义了 TOKEN（如 NUMBER, ADD）
    #include "y.tab.h"
    //  extern 声明，告诉 Flex yylex 已经在别处定义了（在 Bison 生成的文件里）
    extern int yylval; // yylval 用于将 Token 的值传递给 Bison
%}

/* 正则表达式定义 */
DIGIT    [0-9]
INTEGER  {DIGIT}+
WHITESPACE [ \t]

%%
    /* 词法规则 */

{INTEGER}    {
        // 将匹配到的字符串转换为整数，并赋值给 yylval
        yylval = atoi(yytext);
        // 返回 NUMBER 这个 Token 给 Bison
        return NUMBER;
    }

"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"\n"        { return NEWLINE; }
{WHITESPACE} { /* 忽略空白字符 */ }

/* 如果遇到不认识的字符，报错 */
.           {
        fprintf(stderr, "Invalid character '%c'\n", yytext[0]);
        exit(1);
    }

%%

// 这个文件不需要 main 函数，main 在 Bison 文件里
```

#### 3. 编译与运行

```bash
# 1. 使用 Bison 生成解析器 (-d 选项会同时生成头文件 y.tab.h，供 Flex 使用)
bison -d calculator.y

# 2. 使用 Flex 生成词法分析器
flex calculator.l

# 3. 编译所有的 C 文件，链接成可执行程序
gcc -o calculator y.tab.c lex.yy.c

# 4. 运行计算器
./calculator
```

**输入示例:**
```
2 + 3 * 4
 = 14
(2 + 3) * 4
 = 20
-5 + 10
 = 5
```

### 总结

*   **Flex** 负责“读单词”，它看到 `"123"` 就知道这是一个 `NUMBER`，看到 `"+"` 就知道是 `ADD`。
*   **Bison** 负责“理解句子结构”，它收到 `NUMBER`, `ADD`, `NUMBER` 这样的序列，就能根据 `expression ADD expression` 的规则，理解这是一个加法表达式，并执行相应的计算动作。
*   它们通过 `yylex()`/`yyparse()` 函数和 `yylval` 变量进行通信。

这个简单的计算器展示了 Flex 和 Bison 协作的核心机制，你可以基于这个框架扩展出更复杂的语法，比如支持变量、函数、控制流等。