#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "parser.h"


int main(int argc, char **argv)
{
        // 示例源代码
    const char *source_code =
        "int x;\n"
        "x = 10;\n"
        "int y = 20;\n"
        "if (x > 5) {\n"
        "    return x + y;\n"
        "}\n"
        "while (x < 100) {\n"
        "    x = x + 1;\n"
        "}";

    printf("源代码:\n%s\n", source_code);

    // 词法分析
    Lexer *lexer = lexer_create(source_code);
    lexer_tokenize(lexer);
    lexer_print_tokens(lexer);

    // 语法分析
    Parser *parser = parser_create(lexer->tokens, lexer->token_count);
    ASTNode *ast = parse_program(parser);

    printf("=== 语法分析结果 (AST) ===\n");
    ast_print(ast, 0);

    // 清理内存
    free(lexer->tokens);
    free(lexer);
    free(parser);
    return 0;
}

/*
git comit message

Format of the commit message
<type>(<scope>): <subject>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>

type 主要包括：

feat (feature)
fix (bug fix)
docs (documentation)
style (formating, missing semi colons, …)
refactor
test (when adding missing tests)
chore (maintain)

fix($compile): couple of unit tests for IE9
*/