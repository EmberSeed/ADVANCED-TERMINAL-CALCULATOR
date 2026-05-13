# Advanced Terminal Calculator 🧮

> **A powerful, standalone scientific calculator for your terminal**  
> *No GUI, no bloat — just pure math at the speed of light.*

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)]()

---

## ✨ Features at a glance

| Category | What you get |
|----------|---------------|
| **Arithmetic** | `+ - * / ^ %` with parentheses & precedence |
| **Trigonometry** | `sin`, `cos`, `tan`, `asin`, `acos`, `atan` (radians & degrees) |
| **Logarithms** | `ln`, `log`, `log2`, `log10`, `logbase(x,base)` |
| **Roots** | `sqrt`, `cbrt`, `root(x,n)` (n‑th root) |
| **Others** | `exp`, `abs`, `floor`, `ceil`, `round`, `fact` |
| **Number bases** | binary `0b1010`, octal `0o77`, hex `0xFF` — plus `toBin`, `toOct`, `toHex` |
| **Implicit multiply** | `2pi`, `3(2+4)`, `(x+1)(x-1)` |
| **Variable `x`** | for differentiation, plotting & Newton solver |
| **Equation solver** | quadratic (exact), cubic (numerical), general (Newton) |
| **Numerical derivative** | compute `f'(x)` for any expression |
| **Temperature** | Celsius ↔ Fahrenheit ↔ Kelvin |
| **Base conversion** | between any bases 2–36 (interactive) |
| **ASCII plotter** | draw `y = f(x)` with `*` characters |
| **Matrix ops (2×2)** | addition, multiplication, determinant, inverse |
| **History** | `hist` shows last 20 calculations |
| **Exit messages** | obfuscated (Base64) – no plaintext in source |

---

## 🚀 Quick start

```bash
# Compile (requires only standard C and math lib)
gcc -o calculator calculator.c -lm

# Run
./calculator          # Linux / macOS
calculator.exe        # Windows (MinGW)
```

---

## 📖 Usage examples

### 1. Expression calculator

```
expr> sqrt(16) + sin(pi/2)
 = 5
expr> 0b1010 + 0xFF
 = 265
expr> root(8,3)
 = 2
expr> logbase(100,10)
 = 2
expr> 3(2+4)
 = 18
```

### 2. Equation solver

```text
--- Quadratic solver ---
a, b, c: 1 -3 2
Two real roots: 2 , 1

--- Cubic solver ---
a, b, c, d: 1 -6 11 -6
Real root(s): 1 2 3

--- General Newton ---
f(x) = x^2 - 2
Initial guess: 1
Root ~ 1.414213562
```

### 3. ASCII graph plotter

```
f(x) = x + 5
xmin xmax: 0 9

      14 |                                                                    **
      13 |                                                                ****
      ... (nice rising line)
```

### 4. Matrix operations (2×2)

```
Enter matrix A (row1: a b, row2: c d):
1 2 3 4
Enter matrix B:
5 6 7 8

A + B =
       6        8
      10       12
Det(A) = -2
Inverse of A:
      -2        1
     1.5     -0.5
```

---

## 🛠️ Commands inside the calculator

| Command | Action |
|---------|--------|
| `hist`  | Show calculation history |
| `back`  | Return to main menu |
| `help`  | (not needed – all functions are listed) |

---

## ⚙️ Installation details

- **No external libraries** – only `libm` (math library) is needed.
- Works on **Linux, macOS, Windows** (MinGW or WSL).
- Compile with any C99‑compatible compiler.

---

## 📁 Repository structure

```
.
├── main.c          # single‑file source code
├── README.md             # this file
└── LICENSE               # MIT license
```

---

## ⚠️ Limitations

- Symbolic math? No – only numeric.
- Cubic solver is numerical; may miss roots for very flat functions.
- Graph resolution: 70×20 characters.
- Matrices are limited to 2×2.
- Base64 exit messages are obfuscation, not real encryption.

---

## 🤝 Contributing

Keep it **simple**, **robust**, and **fast**.  
Pull requests and issues are welcome.

---

## 📄 License

**MIT** – free for any use. No warranty.

---

## 🧠 Credits

Inspired by the philosophy of **Dennis Ritchie**:  
*“Make it work, make it right, make it fast.”*

---

**Happy calculating!** 🎉
