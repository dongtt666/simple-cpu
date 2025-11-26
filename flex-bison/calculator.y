%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex(void);
    void yyerror(const char *s);
%}

/* 定义TOKEN */
%token NUMBER
%token ADD SUB MUL DIV LPAREN RPAREN NEWLINE

/* 优先级和结合性 */
%left ADD SUB
%left MUL DIV
%left UMINUS

%start line

%%

line
    : /* empty */
    | expression NEWLINE {
        printf("Result: %d\n", $1);
    }
    | line expression NEWLINE {
        printf("Result: %d\n", $2);
    }
    ;

expression
    : NUMBER { $$ = $1; }
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
    | SUB expression %prec UMINUS { $$ = -$2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Simple Calculator. Enter expressions (Ctrl+D to exit):\n");
    yyparse();
    return 0;
}