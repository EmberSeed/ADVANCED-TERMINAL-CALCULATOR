# 🧮 Advanced Terminal Calculator

A zero-dependency scientific calculator written in C.

This project combines an expression parser, scientific functions, numerical methods, equation solving, graph plotting, and basic matrix operations into a single self-contained terminal application.

The entire implementation is contained in a single source file and depends only on the C standard library and `libm`.

---

## ✨ Features

### 1. Arithmetic & Expressions

Supported operators:

* `+`
* `-`
* `*`
* `/`
* `%`
* `^`

Features:

* Full operator precedence
* Parentheses
* Implicit multiplication
* Right-associative exponentiation

Examples:

```text
2+3*4          -> 14
(2+3)*4        -> 20
2pi            -> 6.28318...
3(2+4)         -> 18
(x+1)(x-1)     -> x²-1
2^3^2          -> 512
10%3           -> 1
```

---

### 2. Trigonometric Functions

Radians:

* `sin`
* `cos`
* `tan`
* `asin`
* `acos`
* `atan`

Degrees:

* `sind`
* `cosd`
* `tand`
* `asind`
* `acosd`
* `atand`

Examples:

```text
sin(pi/2)      -> 1
cos(0)         -> 1
tan(pi/4)      -> 1

sind(90)       -> 1
cosd(180)      -> -1
tand(45)       -> 1
```

---

### 3. Logarithmic & Exponential Functions

Supported:

* `ln`
* `log`
* `log10`
* `log2`
* `logbase(x,base)`
* `exp`

Examples:

```text
ln(e)              -> 1
log10(1000)        -> 3
log2(1024)         -> 10
logbase(81,3)      -> 4
exp(1)             -> 2.71828...
```

---

### 4. Roots & Powers

Supported:

* `sqrt`
* `cbrt`
* `root(x,n)`
* `^`

Examples:

```text
sqrt(16)       -> 4
cbrt(27)       -> 3
root(8,3)      -> 2
2^10           -> 1024
```

---

### 5. Other Mathematical Functions

Supported:

* `abs`
* `floor`
* `ceil`
* `round`
* `fact`

Examples:

```text
abs(-10)       -> 10
floor(3.9)     -> 3
ceil(3.1)      -> 4
round(3.6)     -> 4
fact(5)        -> 120
```

---

### 6. Number Base Support

Input formats:

```text
0b1010
0o77
0xFF
```

Conversions:

* `toBin(x)`
* `toOct(x)`
* `toHex(x)`

Examples:

```text
0b1010 + 5     -> 15
0xFF           -> 255

toBin(10)      -> 1010
toOct(64)      -> 100
toHex(255)     -> FF
```

Interactive converter:

* Any base from 2 to 36
* Menu option 5

---

### 7. Equation Solving

#### Quadratic

```text
ax² + bx + c = 0
```

Supports:

* Two real roots
* Double roots
* Complex roots

Example:

```text
x² - 3x + 2 = 0

Roots:
2
1
```

#### Cubic

```text
ax³ + bx² + cx + d = 0
```

Numerical real root detection.

Example:

```text
x³ - 6x² + 11x - 6 = 0

Roots:
1
2
3
```

#### General Equation Solver

Uses Newton-Raphson iteration.

Example:

```text
f(x) = x² - 2

Initial guess: 1

Root ≈ 1.414213562
```

---

### 8. Numerical Differentiation

Computes:

```text
f'(x)
```

using a central difference method.

Example:

```text
f(x) = x²

x = 2

f'(2) = 4
```

---

### 9. Temperature Conversion

Supported:

* Celsius ↔ Fahrenheit
* Celsius ↔ Kelvin
* Fahrenheit ↔ Kelvin

Examples:

```text
0°C      -> 32°F
100°C    -> 373.15K
273.15K  -> 0°C
```

---

### 10. ASCII Graph Plotter

Plot mathematical functions directly in the terminal.

Example:

```text
f(x) = x²
range = -5 .. 5
```

Features:

* 70×20 plotting area
* No GUI required
* Works over SSH

---

### 11. Matrix Operations (2×2)

Supported:

* Addition
* Multiplication
* Determinant
* Inverse

Example:

```text
A = [1 2]
    [3 4]

det(A) = -2
```

---

### 12. History & Session Memory

Commands:

```text
hist
back
```

Features:

* Stores last 20 calculations
* Session memory
* No files written to disk

Example:

```text
1: 2+2 = 4
2: sqrt(16) = 4
3: sin(pi/2) = 1
```

---

## 🚀 Build

### Linux / macOS

```bash
gcc main.c -o calculator -lm
./calculator
```

### Windows (MinGW)

```bash
gcc main.c -o calculator.exe -lm
calculator.exe
```

Requirements:

* GCC, Clang, or MinGW
* Standard C library
* Math library (`libm`)

No package manager.

No external libraries.

No runtime installation.

---

## 📖 Usage

Run the executable.

You will see:

```text
1. Expression Calculator
2. Equation Solver
3. Numerical Differentiation
4. Temperature Conversion
5. Number Base Conversion
6. Special Functions
7. ASCII Graph Plotter
8. Matrix Operations
9. Exit
```

Choose a menu option and follow the prompts.

---

## ⚙️ Design Notes

This project intentionally uses a single source file.

Reasons:

* Easy distribution
* Simple compilation
* No build-system complexity
* Entire implementation can be inspected in one place
* Suitable for learning parsers and numerical methods

The goal is portability and simplicity rather than enterprise-scale architecture.

---

## 📊 Comparison

| Feature                    | bc      | Python         | qalc | Advanced Terminal Calculator |
| -------------------------- | ------- | -------------- | ---- | ---------------------------- |
| Scientific Functions       | Partial | Yes            | Yes  | Yes                          |
| Implicit Multiplication    | No      | No             | Yes  | Yes                          |
| Binary / Octal / Hex Input | No      | Yes            | Yes  | Yes                          |
| Equation Solving           | No      | With Libraries | Yes  | Yes                          |
| Numerical Differentiation  | No      | With Libraries | Yes  | Yes                          |
| Matrix Operations          | No      | With Libraries | Yes  | Yes                          |
| ASCII Graphs               | No      | With Libraries | No   | Yes                          |
| Single Executable          | Yes     | No             | No   | Yes                          |
| External Runtime Required  | No      | Yes            | Yes  | No                           |
| Works Over SSH             | Yes     | Yes            | Yes  | Yes                          |

---

## ⚠️ Limitations

* Uses IEEE-754 double precision
* Numerical rather than symbolic mathematics
* Matrix support is limited to 2×2
* Session history is not persisted to disk
* ASCII plotting is intended for rough visualization

---

## 📄 License

MIT License
