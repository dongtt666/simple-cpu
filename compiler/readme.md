# 简单编程语言语法规则

## 1. 语言特性
- **变量声明**：支持定义变量并存储在寄存器中。
- **算术操作**：支持加法、减法、乘法、除法。
- **逻辑操作**：支持与、或、异或。
- **移位操作**：支持算术和逻辑移位。
- **条件跳转**：支持基于标志位（ZF、NF、CF）的条件跳转。
- **输入输出**：支持从内存加载数据和存储数据。

---

## 2. 语法规则

### 2.1 变量声明：
```plaintext
<var> = <value>：将变量初始化为某个值。
```

### 2.2 算术表达式：
```plaintext
<var> = <var> + <var>：加法。
<var> = <var> - <var>：减法。
```

### 2.3 逻辑表达式：
```plaintext
<var> = <var> & <var>：按位与。
<var> = <var> | <var>：按位或。
```

### 2.4 移位操作：
```plaintext
<var> = <var> << <value>：逻辑左移。
<var> = <var> >> <value>：逻辑右移。
```

### 2.5 条件跳转：
```plaintext
if <condition> goto <label>：条件跳转。
```

### 2.6 输入输出：(暂未支持)
```plaintext
get <var>：从内存加载数据到变量。
put <var>：将变量存储到内存。
```

## 3. 示例程序
以下是一个完整的示例程序，用于计算 (A + B) * C 并将结果存储到内存地址 0x10：
```plaintext
A = 5
B = 10
C = 2
result = 0

result = A + B
result = result * C

print result
```