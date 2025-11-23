#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assembler.h"

static int assemble_file(const char *file_in, const char *file_out)
{
    FILE *input = fopen(file_in, "r");
    FILE *output = fopen(file_out, "wb");
    if (!input || !output) {
        fprintf(stderr, "Unable to open file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        char mnemonic[16], op1[16], op2[16], op3[16];
        int num_operands = sscanf(line, "%s %[^,], %[^,], %s", mnemonic, op1, op2, op3);
        uint16_t binary_instruction = 0;
        if (num_operands == 4) {
            binary_instruction = assemble(mnemonic, op1, op2, op3);
        } else if (num_operands == 3) {
            binary_instruction = assemble(mnemonic, NULL, op1, op2);
        } else if (num_operands == 1) {
            binary_instruction = assemble(mnemonic, NULL, NULL, NULL);
        } else {
            fprintf(stderr, "Unable to parse instruction: %s\n", line);
            fclose(input);
            fclose(output);
            return -1;
        }
        fwrite(&binary_instruction, sizeof(uint16_t), 1, output);
    }
    fclose(input);
    fclose(output);
    printf("Assembly complete: %s.bin\n", file_out);
    return 0;
}

static int disassemble_file(const char *file_in, const char *file_out)
{
    FILE *input = fopen(file_in, "rb");
    FILE *output = fopen(file_out, "w");
    if (!input || !output) {
        perror("Unable to open file");
        return -1;
    }

    uint16_t instruction;
    while (fread(&instruction, sizeof(uint16_t), 1, input)) {
        char output_line[256];
        disassemble(instruction, output_line);
        fprintf(output, "%s\n", output_line);
    }

    fclose(input);
    fclose(output);
    printf("Disassembly complete: %s\n", file_out);
    return 0;
}

// 主程序
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <assemble|disassemble> <filename>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "assemble") == 0) {
        // Assembly mode
        assemble_file(argv[2], "a.bin");
    } else if (strcmp(argv[1], "disassemble") == 0) {
        // Disassembly mode
        disassemble_file(argv[2], "a.asm");
    } else {
        fprintf(stderr, "Unknown parameter: %s\n", argv[1]);
        return 1;
    }

    return 0;
}