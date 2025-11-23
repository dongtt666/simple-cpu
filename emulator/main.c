#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "emulator.h"

int main(int argc, char** argv)
{
    cpu_t cpu;

    if (argc < 2) {
        printf("Usage: %s <program_file>\n", argv[0]);
        return 1;
    }

    if (cpu_init(&cpu) != 0) {
        fprintf(stderr, "Failed to initialize CPU\n");
        return 1;
    }

    if (cpu_load_program(&cpu, argv[1]) < 0) {
        fprintf(stderr, "Failed to load program: %s\n", argv[1]);
        return 1;
    }

    cpu_exec(&cpu);

    printf("Program executed successfully.\n");
    return 0;
}