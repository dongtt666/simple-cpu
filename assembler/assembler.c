#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "opcodes.h"
#include "assembler.h"

static struct instruction_st s_inst_table[] = {
    {"NOP",     NOP,    OP_TYPE_NONE},
    {"HALT",    HALT,   OP_TYPE_NONE},
    {"LOAD",    LOAD,   OP_TYPE_RI},
    {"STORE",   STORE,  OP_TYPE_RI},
    {"LDIH",    LDIH,   OP_TYPE_I},
    {"ADD",     ADD,    OP_TYPE_R},
    {"ADDI",    ADDI,   OP_TYPE_I},
    {"ADDC",    ADDC,   OP_TYPE_R},
    {"SUB",     SUB,    OP_TYPE_R},
    {"SUBI",    SUBI,   OP_TYPE_I},
    {"SUBC",    SUBC,   OP_TYPE_R},
    {"CMP",     CMP,    OP_TYPE_R},     // 特殊处理
    {"AND",     AND,    OP_TYPE_R},
    {"OR",      OR,     OP_TYPE_R},
    {"XOR",     XOR,    OP_TYPE_R},
    {"SLL",     SLL,    OP_TYPE_RI},
    {"SRL",     SRL,    OP_TYPE_RI},
    {"SLA",     SLA,    OP_TYPE_RI},
    {"SRA",     SRA,    OP_TYPE_RI},
    {"JUMP",    JUMP,   OP_TYPE_I},     // 特殊处理
    {"JMPR",    JMPR,   OP_TYPE_I},
    {"BZ",      BZ,     OP_TYPE_I},
    {"BNZ",     BNZ,    OP_TYPE_I},
    {"BN",      BN,     OP_TYPE_I},
    {"BNN",     BNN,    OP_TYPE_I},
    {"BC",      BC,     OP_TYPE_I},
    {"BNC",     BNC,    OP_TYPE_I},
};

int get_register_number(const char *reg) {
    const char *registers[] = {
        "gr0", "gr1", "gr2", "gr3", "gr4", "gr5", "gr6", "gr7"
    };
    int num_registers = sizeof(registers) / sizeof(registers[0]);

    if (!reg) {
        return 0;
    }

    for (int i = 0; i < num_registers; i++) {
        if (strcmp(reg, registers[i]) == 0) {
            return i;
        }
    }

    return -1;
}

uint16_t assemble(const char *mnemonic, const char *op1, const char *op2, const char *op3) {
    uint16_t instruction = 0;

    // 查找指令
    int found = 0;
    for (int i = 0; i < ARRAY_SIZE(s_inst_table); i++) {
        if (strcmp(mnemonic, s_inst_table[i].mnemonic) == 0) {
            instruction |= (s_inst_table[i].opcode << 11);
            found = 1;

            // 根据指令类型生成指令
            switch (s_inst_table[i].type) {
                case OP_TYPE_R: // R 型指令
                    instruction |= (get_register_number(op1) << 8);
                    instruction |= (get_register_number(op2) << 4);
                    instruction |= (get_register_number(op3) & 0xF);
                    break;
                case OP_TYPE_I: // I 型指令
                    instruction |= (get_register_number(op1) << 8);
                    instruction |= (atoi(op2) & 0xF) << 4;
                    instruction |= (atoi(op3) & 0xF);
                    break;
                case OP_TYPE_RI: // RI 型指令
                    instruction |= (get_register_number(op1) << 8);
                    instruction |= (get_register_number(op2) << 4);
                    instruction |= (atoi(op3) & 0xF);
                    break;
                case OP_TYPE_NONE: // 无操作数指令
                    break;
                default:
                    fprintf(stderr, "Unknown instruction type: %d\n", s_inst_table[i].type);
                    exit(1);
            }
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Unknown instruction: %s\n", mnemonic);
        exit(1);
    }

    return instruction;
}

void disassemble(uint16_t instruction, char *output) {
    uint16_t opcode = (instruction >> 11) & 0x1F;
    uint16_t op1 = (instruction >> 8) & 0x7;
    uint16_t op2 = (instruction >> 4) & 0xF;
    uint16_t op3 = instruction & 0xF;

    // 查找指令
    int found = 0;
    for (int i = 0; i < ARRAY_SIZE(s_inst_table); i++) {
        if (opcode == s_inst_table[i].opcode) {
            found = 1;

            // 根据指令类型生成汇编代码
            switch (s_inst_table[i].type) {
                case OP_TYPE_R: // R 型指令
                    if (strcmp(s_inst_table[i].mnemonic, "CMP") == 0) {
                        sprintf(output, "CMP gr%d, gr%d", op2, op3);
                    } else {
                        sprintf(output, "%s gr%d, gr%d, gr%d", s_inst_table[i].mnemonic, op1, op2, op3);
                    }
                    break;
                case OP_TYPE_I: // I 型指令
                    if (strcmp(s_inst_table[i].mnemonic, "JUMP") == 0) {
                        sprintf(output, "JUMP %d, %d", op2, op3);
                    } else {
                        sprintf(output, "%s gr%d, %d, %d", s_inst_table[i].mnemonic, op1, op2, op3);
                    }
                    sprintf(output, "%s gr%d, %d, %d", s_inst_table[i].mnemonic, op1, op2, op3);
                    break;
                case OP_TYPE_RI: // RI 型指令
                    sprintf(output, "%s gr%d, gr%d, %d", s_inst_table[i].mnemonic, op1, op2, op3);
                    break;
                case OP_TYPE_NONE: // 无操作数指令
                    sprintf(output, "%s", s_inst_table[i].mnemonic);
                    break;
                default:
                    sprintf(output, "Unknown instruction type: %d", s_inst_table[i].type);
                    break;
            }
            break;
        }
    }

    if (!found) {
        sprintf(output, "Unknown instruction: 0x%04X", instruction);
    }
}