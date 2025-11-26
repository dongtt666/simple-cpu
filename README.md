# simplecpu
Simple CPU and compiler，the code has only undergone simple testing and can be considered a toy project, intended solely for learning and reference purposes.


## emulator
The simulator only implements the basic operations of the CPU instruction set and lacks other complete functionalities, such as memory management. This instruction set is from a university laboratory course.


## assembler
The assembler's function is to convert assembly code into executable code for the simulator, as well as disassemble the executable binary file back into assembly code.


## compiler
Since the CPU simulator only implements instruction set execution and lacks full functionality such as memory management, the compiler has only implemented lexical analysis and syntax analysis, with code generation not yet implemented.

## todo
In the future, a JIT interpreter may be implemented for direct interpretation and execution, or after achieving a complete CPU simulator, conversion to corresponding assembly code may be realized.
