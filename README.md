# Simple Assembly Interpreter

This is a simple assembly interpreter with 2 registers written in C.
This program can read and execute the following instructions -

-   ADD x y
-   SUB x y
-   MUL x y
-   DIV x y
-   AND x y
-   OR x y
-   NOT x
-   LOAD x y
-   STR x y

### Sample Input/Output

**Input**

```
ADD x 5
ADD y 7
MUL x y
DIV x 5

ADD y 0x4
ADD y x
SUB y 0x1

NOT x
OR x 1

ADD x 0x7
ADD x 1

AND x 1
MUL x 3


STR 0x7 x
LOAD x 33
STR 0x1 99

```

**Output**

```
Register X: 33 | Register Y: 17
Memory locations: 99 1 2 3 4 5 6 3
```

### Usage

**Compile**

```bash
make
```

**Run with program file**

```bash
./main <program.asm>
```

> Replace `<program.asm>` with your assembly file

**Run in interactive mode**

```bash
./main -i
```

> You can pass -v flag for verbose output
