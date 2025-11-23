## Design target
* RISC CPU
* Data path 16b
* Data memory
    >- 2^8 X 16b
* Operation memory:
    >- 2^8 X 16b
    >- Sizeofoperationset:2^5
* General register
    >- 8X16b
* Flags
    >- NF,ZF,CF
* Control
    >- Clock,reset,enable,start
* Testing
    >- 4bitselectionfor16bitoutput


## Operation field
15              11 10                8 7                 4 3                  0
+-----------------+-------------------+-------------------+-------------------+
| Op code (5 bit) | Operand 1 (3 bit) | Operand 2 (4 bit) | Operand 3 (4 bit) |
+-----------------+-------------------+-------------------+-------------------+
* R type (register type)
    - r1 r2 r3
    - r1 r2 -
* I type (Immediate type)
    - r1 val2 val3
    - \- val2 val3
* RI type
    - r1 r2 val3


## An example of operation codes (assemble language)
```c
    In C code:
    Y= A+B
    M[gr0+2] =M[gr0+1]+M[gr0+0]
```
```asm
    LOAD gr1, gr0, 0
    LOAD gr2, gr0, 1
    ADD gr3, gr1, gr2
    STORE gr3, gr0, 2
    HALT
```

## Access memory and register
* Access register:
    >- Ex. gr[r1],
    >- r1 for simplification
    >- [r1]/r1cgets register number
* Access memory:
    >- Ex. m[r2+val3],
    >- [r2+val3]/[gr(r2)+val3] gets address of memory
* Access immediate data:
    >- Ex. {val2, val3},
    >- MSB: val2, LSB:val3


## Data transfer & Arithmetic
| mnemonic | operand1 | operand2 | operand3 | op code | operation |
| -------- | -------- | -------- | -------- | ------- | --------- |
| NOP      |          |          |          |  00000  | no operation |
| HALT     |          |          |          |  00001  | halt |
| LOAD     |    r1    |    r2    |    val3  |  00010  | gr[r1]<-m[r2+val3] |
| STORE    |    r1    |    r2    |    val3  |  00011  | m[r2+val3]<-r1 |
| LDIH     |    r1    |   val2   |    val3  |  10000  | r1<-r1+{val2, val3, 0000_0000} |
| ADD      |    r1    |    r2    |    r3    |  01000  | r1<-r2+r3 |
| ADDI     |    r1    |    val2  |    val3  |  01001  | r1<-r1+{val2, val3} |
| ADDC     |    r1    |    r2    |    r3    |  10001  | r1<-r2+r3+CF ｜
| SUB      |    r1    |    r2    |    r3    |  01000  | r1<-r2-r3 |
| SUBI     |    r1    |    val2  |    val3  |  01001  | r1<-r1-{val2, val3} |
| SUBC     |    r1    |    r2    |    r3    |  10001  | r1<-r2-r3-CF ｜
| CMP      |          |    r2    |    r3    |  01100  |  r2-r3; set CF,ZF and NF |


## Logical / shift
| mnemonic | operand1 | operand2 | operand3 | op code | operation |
| -------- | -------- | -------- | -------- | ------- | --------- |
| AND      |    r1    |    r2    |    r3    |  01101  | r1<-r2 and r3 |
| OR       |    r1    |    r2    |    r3    |  01110  | r1<-r2 or r3 |
| XOR      |    r1    |    r2    |    r3    |  01111  | r1<-r2 xor r3 |
| SLL      |    r1    |    r2    |    val3  |  00100  | r1<-r2 shift left logical (val3 bit shift) |
| SRL      |    r1    |    r2    |    val3  |  00110  | r1<-r2 shift right logical (val3 bit shift) |
| SLA      |    r1    |    r2    |    val3  |  00101  | r1<-r2 shift left arithmetical (val3 bit shift) |
| SRA      |    r1    |    r2    |    val3  |  00111  | r1<-r2 shift right arithmetical (val3 bit shift) |

* operations
    >- Logical shift: 1001 SRL 2b -> 0010 (append 0) Arithmetical
    >- shift: 1001 SRA 2b -> 1110 (keep the sign)


## Control
| mnemonic | operand1 | operand2 | operand3 | op code | operation |
| -------- | -------- | -------- | -------- | ------- | --------- |
| JUMP     |          |   val2   |   val3   |  11000  | jump to {val2, val3} |
| JMPR     |   r1     |   val2   |   val3   |  11001  | jump to r1+{val2, val3} |
| BZ       |   r1     |   val2   |   val3   |  11010  | if ZF=1 branch to r1+{val2, val3}  |
| BNZ      |   r1     |   val2   |   val3   |  11011  | if ZF=0 branch to r1+{val2, val3}  |
| BN       |   r1     |   val2   |   val3   |  11100  | if NF=1 branch to r1+{val2, val3}  |
| BNN      |   r1     |   val2   |   val3   |  11101  | if NF!=1 branch to r1+{val2, val3} |
| BC       |   r1     |   val2   |   val3   |  11110  | if CF=1 branch to r1+{val2, val3}  |
| BNC      |   r1     |   val2   |   val3   |  11110  | if CF!=1 branch to r1+{val2, val3} |

* Flag registers:
    >- Used for control operations (conditional branch)
    >- ZF (zero flag), NF (negative flag), CF (carry flag)


## Storage

* Outside CPU
    - Instruction storage
        - Instruction memory (2^5 X 16b)
    - Data storage
        - Data memory (2^8 X 16b)

* Inside CPU
    - Instruction storage
        - Stage instruction registers
            - id_ir(16b x1), ex_ir (16b x 1), mem_ir (16b x 1), wb_ir (16b x 1)
    - Data storage
        - General registers
            - Storage for operand data (2^3 X 16b)
        - Stage data registers
            - @ID: reg_A (16b x 1), reg_B (16b x 1), smdr (16b x 1),
            - @EX : reg_C (16b x 1), flag (1b x3), dw (1b x1), smdr1 (16b x 1),
            - @WB: reg_C1 (16b x 1)