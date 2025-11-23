#ifndef _OPCODES_H_20251117_
#define _OPCODES_H_20251117_

#define OPCODES(XX)   \
    XX(0b00000, NOP,              "NOP"               ) \
    XX(0b00001, HALT,             "HALT"              ) \
    XX(0b00010, LOAD,             "LOAD"              ) \
    XX(0b00011, STORE,            "STORE"             ) \
    XX(0b10000, LDIH,             "LDIH"              ) \
    XX(0b01000, ADD,              "ADD"               ) \
    XX(0b01001, ADDI,             "ADDI"              ) \
    XX(0b10001, ADDC,             "ADDC"              ) \
    XX(0b01010, SUB,              "SUB"               ) \
    XX(0b01011, SUBI,             "SUBI"              ) \
    XX(0b10010, SUBC,             "SUBC"              ) \
    XX(0b01100, CMP,              "CMP"               ) \
    XX(0b01101, AND,              "AND"               ) \
    XX(0b01110, OR,               "OR"                ) \
    XX(0b01111, XOR,              "XOR"               ) \
    XX(0b00100, SLL,              "SLL"               ) \
    XX(0b00110, SRL,              "SRL"               ) \
    XX(0b00101, SLA,              "SLA"               ) \
    XX(0b00111, SRA,              "SRA"               ) \
    XX(0b11000, JUMP,             "JUMP"              ) \
    XX(0b11001, JMPR,             "JMPR"              ) \
    XX(0b11010, BZ,               "BZ"                ) \
    XX(0b11011, BNZ,              "BNZ"               ) \
    XX(0b11100, BN,               "BN"                ) \
    XX(0b11101, BNN,              "BNN"               ) \
    XX(0b11110, BC,               "BC"                ) \
    XX(0b11111, BNC,              "BNC"               ) \
    XX(0b10011, TRAP,             "TRAP"              ) \
    XX(0b10100, RESEVE1,          "RESEVE1"           ) \
    XX(0b10101, RESEVE2,          "RESEVE2"           ) \
    XX(0b10110, RESEVE3,          "RESEVE3"           ) \
    XX(0b10111, RESEVE4,          "RESEVE4"           ) \


#define OPCODES_ENUM_GEN(v, n, s)  n = (v),
enum op_codes {
    OPCODES(OPCODES_ENUM_GEN)
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif