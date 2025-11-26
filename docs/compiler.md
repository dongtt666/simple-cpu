# Simple C Language Compiler Documentation

## 📋 Compiler Features

### Supported Syntax Features

#### 1. Variable Declaration and Initialization
```c
int x;           // Variable declaration
int y = 20;      // Variable declaration with initialization
```

#### 2. Assignment Statements
```c
x = 10;          // Simple assignment
x = y + 5;       // Expression assignment
```

#### 3. Arithmetic Operations
```c
a + b            // Addition
a - b            // Subtraction
a * b            // Multiplication
a / b            // Division
```

#### 4. Comparison Operations
```c
a == b           // Equal to
a != b           // Not equal to
a < b            // Less than
a > b            // Greater than
a <= b           // Less than or equal to
a >= b           // Greater than or equal to
```

#### 5. Control Flow Statements
```c
if (condition) { ... }          // If statement
if (condition) { ... } else { ... }  // If-else statement
while (condition) { ... }       // While loop
```

#### 6. Function Return
```c
return value;    // Return with value
return;          // Return without value
```

#### 7. Code Blocks
```c
{
    statement1;
    statement2;
    // ... multiple statements
}
```

## 📁 Project Structure

```
compiler/
├── token.c token.h         # Token defination
├── lexer.h/lexer.c         # Lexical Analyzer
├── parser.h/parser.c       # Syntax Parser
├── codegen.h/codegen.c     # Code Generator
├── main.c                  # Main Program
├── Makefile
```

## 🎯 Current Limitations

- No function definitions and calls
- No arrays and pointers
- No floating-point numbers
- No string operations
- No preprocessor directives
- Global variable scope only

## 🔮 Future Enhancements

- [ ] Support function definitions and calls
- [ ] Add local variables and scoping
- [ ] Support array types
- [ ] Add basic type checking
- [ ] Support simple standard library functions

## 🏗️ Architecture Overview

### Compiler Pipeline
```
Source Code
    → Lexical Analysis (Tokenizer)
    → Syntax Analysis (Parser)
    → Abstract Syntax Tree (AST)
    → Code Generation
    → Assembly Output
```

### Key Components

1. **Lexer**: Converts source code into tokens
2. **Parser**: Builds AST from token stream
3. **Code Generator**: Converts AST to x86-64 assembly
4. **AST Nodes**: Represent program structure for code generation

This compiler contains the core components of modern compilers: lexical analysis, syntax analysis, semantic analysis (AST generation), and code generation. It serves as an excellent foundation for learning compiler design principles.