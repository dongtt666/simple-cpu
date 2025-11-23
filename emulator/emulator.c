#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "opcodes.h"
#include "emulator.h"

static const char* s_op_code_str[] = {
    OPCODES(OPCODES_ARRAY_GEN)
};

int cpu_init(cpu_t *cpu)
{
    cpu->pc = 0;
    cpu->nf = 0;
    cpu->zf = 0;
    cpu->cf = 0;
    memset(cpu->regs, 0, sizeof(cpu->regs));
    memset(cpu->mem_inst, 0, sizeof(cpu->mem_inst));
    memset(cpu->mem_data, 0, sizeof(cpu->mem_data));
    return 0;
}

int cpu_load_program(cpu_t *cpu, const char* filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }

    size_t read_count = fread(cpu->mem_inst, sizeof(uint16_t), MEMORY_SIZE, file);
    fclose(file);

    return read_count;
}

void cpu_exec(cpu_t *cpu)
{
    while (1) {
        uint16_t instruction = cpu->mem_inst[cpu->pc];
        uint8_t opcode = (instruction >> 11) & 0x1F;
        uint8_t op1 = (instruction >> 8) & 0x07;
        uint8_t op2 = (instruction >> 4) & 0x0F;
        uint8_t op3 = instruction & 0x0F;

        cpu_dump(cpu);  // Dump CPU state before executing instruction

        switch (opcode) {
            case NOP: { cpu->pc++; break; }
            case HALT: { return; }
            case LOAD: {
                cpu->regs[op1] = cpu->mem_data[cpu->regs[op2] + op3];
                cpu->pc++;
                break;
            }
            case STORE: {
                cpu->mem_data[cpu->regs[op2] + op3] = cpu->regs[op1];
                cpu->pc++;
                break;
            }
            case LDIH: {
                cpu->regs[op1] = cpu->regs[op1] + ((op2 << 12 | op3 << 8) & 0xFF00);
                cpu->pc++;
                break;
            }
            case ADD: {
                cpu->regs[op1] = cpu->regs[op2] + cpu->regs[op3];
                cpu->pc++;
                break;
            }
            case ADDI: {
                cpu->regs[op1] = cpu->regs[op1] + (op2 << 4 | op3);
                cpu->pc++;
                break;
            }
            case ADDC: {
                uint32_t result = cpu->regs[op2] + cpu->regs[op3] + cpu->cf;
                cpu->regs[op1] = (uint16_t)result;
                cpu->cf = (result > 0xFFFF) ? 1 : 0;
                cpu->pc++;
                break;
            }
            case SUB: {
                cpu->regs[op1] = cpu->regs[op2] - cpu->regs[op3];
                cpu->pc++;
                break;
            }
            case SUBI: {
                cpu->regs[op1] = cpu->regs[op2] - (op2 << 4 | op3);
                cpu->pc++;
                break;
            }
            case SUBC: {
                uint32_t result = cpu->regs[op2] - cpu->regs[op3] - cpu->cf;
                cpu->regs[op1] = (uint16_t)result;
                cpu->cf = (result > 0xFFFF) ? 1 : 0;
                cpu->pc++;
                break;
            }
            case CMP: {
                uint16_t result = cpu->regs[op2] - cpu->regs[op3];
                cpu->zf = (result == 0) ? 1 : 0;
                cpu->nf = (result & 0x8000) ? 1 : 0;
                cpu->pc++;
                break;
            }
            case AND: {
                cpu->regs[op1] = cpu->regs[op2] & cpu->regs[op3];
                cpu->pc++;
                break;
            }
            case OR: {
                cpu->regs[op1] = cpu->regs[op2] | cpu->regs[op3];
                cpu->pc++;
                break;
            }
            case XOR: {
                cpu->regs[op1] = cpu->regs[op2] ^ cpu->regs[op3];
                cpu->pc++;
                break;
            }
            case SLL: {
                cpu->regs[op1] = cpu->regs[op2] << op3;
                cpu->pc++;
                break;
            }
            case SRL: {
                cpu->regs[op1] = cpu->regs[op2] >> op3;
                cpu->pc++;
                break;
            }
            case SLA: {
                cpu->regs[op1] = cpu->regs[op2] << op3;
                cpu->pc++;
                break;
            }
            case SRA: {
                cpu->regs[op1] = (cpu->regs[op2] >> op3) | (cpu->regs[op2] & 0x8000 ? ((0xFFFF) << (16 - op3)) : 0);
                cpu->pc++;
                break;
            }
            case JUMP: {
                cpu->pc = op2 << 4 | op3;
                break;
            }
            case JMPR: {
                cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                break;
            }
            case BZ: {
                if (cpu->zf) {
                    cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                } else {
                    cpu->pc++;
                }
                break;
            }
            case BNZ: {
                if (!cpu->zf) {
                    cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                } else {
                    cpu->pc++;
                }
                break;
            }
            case BN: {
                if (cpu->nf) {
                    cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                } else {
                    cpu->pc++;
                }
                break;
            }
            case BNN: {
                if (!cpu->nf) {
                    cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                } else {
                    cpu->pc++;
                }
                break;
            }
            case BC: {
                if (cpu->cf) {
                    cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                } else {
                    cpu->pc++;
                }
                break;
            }
            case BNC: {
                if (!cpu->cf) {
                    cpu->pc = cpu->regs[op1] + (op2 << 4 | op3);
                } else {
                    cpu->pc++;
                }
                break;
            }

            default:
                // Handle unknown opcode
                return;
        }
    }
}

void cpu_dump(cpu_t *cpu)
{
    uint16_t instruction = cpu->mem_inst[cpu->pc];
    uint8_t opcode = (instruction >> 11) & 0x1F;
    uint8_t op1 = (instruction >> 8) & 0x07;
    uint8_t op2 = (instruction >> 4) & 0x0F;
    uint8_t op3 = instruction & 0x0F;

    printf("PC: 0x%04X -> ", cpu->pc);

    // 遍历每一位，从高位到低位输出
    for (int i = 15; i >= 0; i--) {
        printf("%d", (instruction >> i) & 1); // 提取第 i 位并打印
        if (i % 4 == 0 && i != 0) {          // 每 4 位加一个空格，便于阅读
            printf(" ");
        }
    }
    printf(" | ");

    printf("Opcode: %s (%d), Op1: %d, Op2: %d, Op3: %d\n",
           s_op_code_str[opcode], opcode, op1, op2, op3);

    printf("    Regs(R0-7): ");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("0x%04X ", cpu->regs[i]);
    }
    printf("NF: %d, ZF: %d, CF: %d\n", cpu->nf, cpu->zf, cpu->cf);

    if (opcode == LOAD || opcode == STORE) {
        uint8_t addr = cpu->regs[op2] + op3;
        printf("    Data Memory: 0x%04X: 0x%04X\n", addr, cpu->mem_data[addr]);
    }
}