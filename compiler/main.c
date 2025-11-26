#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"

#define MAX_ASM_LENGTH 65536

// 示例源代码
const char *source_code =
        "int x;\n"
        "x = 10;\n"
        "int y = 20;\n"
        "if (x >= 5) {\n"
        "    return x + y;\n"
        "}\n"
        "while (x < 100) {\n"
        "    x = x + 1;\n"
        "}";

int main(int argc, char **argv)
{
    printf("Source codes:\n%s\n", source_code);

    // Lexical analysis
    struct lexer_st *lexer = lexer_create(source_code);
    if (lexer_tokenize(lexer)) {
        fprintf(stderr, "Lexical analysis error!\n");
        lexer_destroy(lexer);
        return 1;
    }

    lexer_print_tokens(lexer);

    // Syntax analysis
    struct parser_st *parser = parser_create(lexer);
    if (parser == NULL) {
        fprintf(stderr, "Failed to create parser!\n");
        lexer_destroy(lexer);
        return 1;
    }

    struct ast_node_st *ast = parse_program(parser);
    if (ast == NULL) {
        fprintf(stderr, "Syntax analysis error!\n");
        parser_destroy(parser);
        lexer_destroy(lexer);
        return 1;
    }

    ast_print_tree(ast);

    // Code generation
    char asm_code[MAX_ASM_LENGTH] = {0};
    struct codegen_context_st *codegen_ctx = codegen_create(ast);
    int len = codegen_generate(codegen_ctx, asm_code, MAX_ASM_LENGTH);
    printf("\nGenerated Assembly Code (%d bytes):\n%s\n", len, asm_code);

    // Clean up memory
    codegen_destroy(codegen_ctx);
    ast_node_destroy(ast);
    parser_destroy(parser);
    lexer_destroy(lexer);
    return 0;
}
