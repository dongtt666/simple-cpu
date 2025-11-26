%{
    #include "calculator.tab.h"
    extern int yylval;
%}

/* 添加这行来禁用 yywrap 功能 */
%option noyywrap

/* 正则表达式定义 */
DIGIT       [0-9]
INTEGER     {DIGIT}+
WHITESPACE  [ \t]

%%

{INTEGER}   {
    yylval = atoi(yytext);
    return NUMBER;
}

"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"\n"        { return NEWLINE; }

{WHITESPACE} { /* 忽略空白 */ }

.           {
    fprintf(stderr, "Lexical error: unrecognized character '%c'\n", yytext[0]);
    return -1;
}

%%

/*
int yywrap(void) {
    return 1; // 返回1表示输入结束
}
*/

/* 不需要main函数 */