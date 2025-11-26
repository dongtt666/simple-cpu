#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "codegen.h"

typedef struct ast_node_st ast_node_t;
typedef struct codegen_context_st codegen_context_t;

/* 函数声明 */
struct codegen_context_st *codegen_create(struct ast_node_st *ast)
{
    codegen_context_t *ctx = (codegen_context_t *)malloc(sizeof(codegen_context_t));
    if (!ctx) {
        return NULL;
    }

    ctx->ast = ast;
    ctx->label_count = 0;
    ctx->stack_offset = 0;
    ctx->temp_var_count = 0;
    return ctx;
}

void codegen_destroy(struct codegen_context_st *ctx)
{
    if (ctx) {
        free(ctx);
    }
}

/* 生成汇编指令 */
static int codegen_emit(codegen_context_t *ctx, char *buffer, int size, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int written = vsnprintf(buffer, size, format, args);
    va_end(args);

    return written;
}

int codegen_generate(struct codegen_context_st *ctx, char *asm_code, int max_length)
{
    int len = 0;

    len += codegen_emit(ctx, asm_code + len, max_length - len,
                 "TODO: generate asm code\n");

    return len;
}