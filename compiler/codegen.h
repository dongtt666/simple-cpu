#ifndef _CODEGEN_H_20251117_
#define _CODEGEN_H_20251117_

#include "parser.h"

/* 代码生成器上下文 */
struct codegen_context_st{
    struct ast_node_st *ast;        /* 抽象语法树根节点 */
    int label_count;                /* 标签计数器 */
    int stack_offset;               /* 当前栈偏移量 */
    int temp_var_count;             /* 临时变量计数器 */
};

/* 函数声明 */
struct codegen_context_st *codegen_create(struct ast_node_st *ast);
int codegen_generate(struct codegen_context_st *ctx, char *asm_code, int max_length);
void codegen_destroy(struct codegen_context_st *ctx);

#endif /* _CODEGEN_H_20251117_ */