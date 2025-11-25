#ifndef _CODEGEN_H_20251117_
#define _CODEGEN_H_20251117_

#define MAX_CODE_LEN 1000

/**
 * @brief 操作码枚举
 * 定义虚拟机的所有指令类型
 */
typedef enum {
    OP_LOAD,    /**< 加载操作：将操作数压入栈 */
    OP_STORE,   /**< 存储操作：将栈顶值存入变量 */
    OP_ADD,     /**< 加法运算：栈顶两个元素相加 */
    OP_SUB,     /**< 减法运算：栈顶两个元素相减 */
    OP_MUL,     /**< 乘法运算：栈顶两个元素相乘 */
    OP_DIV,     /**< 除法运算：栈顶两个元素相除 */
    OP_PRINT,   /**< 打印操作：输出栈顶值 */
    OP_JMP,     /**< 无条件跳转：跳转到指定地址 */
    OP_JZ       /**< 条件跳转（为零跳转）：栈顶为零时跳转 */
} OpCode;

/**
 * @brief 指令结构体
 * 表示虚拟机的一条指令
 */
typedef struct {
    OpCode opcode;  /**< 操作码，指定要执行的操作类型 */
    int operand;    /**< 操作数，指令的参数（如常量值、变量索引、跳转地址等） */
} Instruction;

/**
 * @brief 代码生成器结构体
 * 管理生成的中间代码（指令序列）
 */
typedef struct {
    Instruction code[MAX_CODE_LEN]; /**< 指令数组，存储生成的代码 */
    int count;                      /**< 当前指令数量 */
} CodeGenerator;

#endif
