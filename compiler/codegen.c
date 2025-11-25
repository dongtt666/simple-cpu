#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

void codegen_init(CodeGenerator *cg) {
    cg->count = 0;
}

void codegen_emit(CodeGenerator *cg, OpCode opcode, int operand) {
    if (cg->count >= MAX_CODE_LEN) {
        fprintf(stderr, "Error: Code buffer overflow\n");
        exit(1);
    }

    cg->code[cg->count].opcode = opcode;
    cg->code[cg->count].operand = operand;
    cg->count++;
}

void codegen_print(CodeGenerator *cg) {
    const char *opcode_names[] = {
        "LOAD", "STORE", "ADD", "SUB", "MUL", "DIV", "PRINT", "JMP", "JZ"
    };

    printf("=== Generated Code ===\n");
    for (int i = 0; i < cg->count; i++) {
        printf("%03d: %-6s %d\n", i, opcode_names[cg->code[i].opcode], cg->code[i].operand);
    }
    printf("======================\n");
}