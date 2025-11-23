#ifndef EMULATOR_H_20251117_
#define EMULATOR_H_20251117_

#include <stdint.h>

/*
15              11 10                8 7                 4 3                  0
+-----------------+-------------------+-------------------+-------------------+
| Op code (5 bit) | Operand 1 (3 bit) | Operand 2 (4 bit) | Operand 3 (4 bit) |
+-----------------+-------------------+-------------------+-------------------+
*/


#define MEMORY_SIZE 256     // 2^8 * 16b
#define NUM_REGISTERS 8     // 8 general-purpose registers

struct cpu_st {
    uint16_t mem_inst[MEMORY_SIZE];     // Instruction memory
    uint16_t mem_data[MEMORY_SIZE];     // Data memory
    uint16_t regs[NUM_REGISTERS];          // general-purpose registers
    uint16_t pc;            // Program counter
    uint16_t nf:1;        // NF flag
    uint16_t zf:1;        // ZF flag
    uint16_t cf:1;        // CF flag
};

typedef struct cpu_st cpu_t;

int cpu_init(cpu_t *cpu);
int cpu_load_program(cpu_t *cpu, const char* filename);
void cpu_exec(cpu_t *cpu);
void cpu_dump(cpu_t *cpu);


#endif