#ifndef ASSEMBLER_H_20251117_
#define ASSEMBLER_H_20251117_

#include <stdint.h>

// 定义指令类型
enum {
    OP_TYPE_NONE,  // 无操作数指令
    OP_TYPE_R,     // R 型指令（寄存器操作）
    OP_TYPE_I,     // I 型指令（立即数操作）
    OP_TYPE_RI,    // RI 型指令（寄存器和立即数混合操作）
};

// 定义指令表
struct instruction_st {
    const char *mnemonic; // 指令名称
    uint16_t opcode;      // 操作码
    int type;             // 指令类
};

int get_register_number(const char *reg);
uint16_t assemble(const char *mnemonic, const char *op1, const char *op2, const char *op3);
void disassemble(uint16_t instruction, char *output);


#endif  // ASSEMBLER_H_20251117_