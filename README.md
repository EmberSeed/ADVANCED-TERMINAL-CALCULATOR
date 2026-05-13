Advanced Terminal Calculator – Complete Documentation
1. Overview
Advanced Terminal Calculator is a powerful, standalone command-line scientific calculator. It runs in any terminal (Linux, macOS, Windows) without a graphical environment. The program provides a rich set of mathematical functions, equation solving, base conversions, matrix operations, ASCII plotting, and more – all with a clean, interactive menu.

2. Features
Full arithmetic: + - * / ^ % and parentheses

Trigonometric functions: sin, cos, tan, asin, acos, atan (radians) and sind, cosd, tand, asind, acosd, atand (degrees)

Logarithms: ln (natural), log (base 10), log2, log10, and logbase(x, base)

Roots: sqrt, cbrt, root(x, n) (n-th root)

Other functions: exp, abs, floor, ceil, round, fact (factorial)

Number literals: Decimal, binary (0b1010), octal (0o77), hexadecimal (0xFF)

Base conversion: toBin(x), toOct(x), toHex(x) (returns decimal representation of digits)

Implicit multiplication: e.g., 2pi, 3(2+4), (x+1)(x-1)

Variable x: used in differentiation, plotting, and Newton solver

History: hist command shows recent calculations

Equation solver: quadratic (exact), cubic (numerical), general (Newton-Raphson)

Numerical differentiation: compute f'(x) for any expression

Temperature conversion: Celsius, Fahrenheit, Kelvin

Number base conversion (interactive): between bases 2–36

ASCII graph plotter: draw y = f(x) with * characters

2×2 matrix operations: addition, multiplication, determinant, inverse

Encrypted exit messages: obfuscated with Base64 (plaintext not visible in source)

3. Installation
Requirements
C compiler (GCC, Clang, or MinGW on Windows)

Standard math library (libm)

Compilation
Linux / macOS

bash
gcc -o calculator calculator.c -lm
Windows (MinGW)

bash
gcc -o calculator.exe calculator.c -lm
No external libraries are required.

Running
bash
./calculator        # Linux/macOS
calculator.exe      # Windows
4. Main Menu
After launching, you see a banner and menu:

text
======================================================
         ADVANCED TERMINAL CALCULATOR - v3.0
   "Make it work, make it right, make it fast."
              -- Dennis Ritchie (in spirit)
======================================================

  1. Expression calculator (with history, variable x)
  2. Equation solver (quadratic/cubic/general)
  3. Numerical differentiation
  4. Temperature conversion
  5. Number base conversion (2-36)
  6. Special functions (sin, sqrt, log, fact, ...)
  7. ASCII Graph plotter
  8. Matrix operations (2x2)
  9. Exit

---------------------------------------------------------
Choice: 
Enter the number of the desired feature. After finishing a sub‑menu, press Enter to return to the main menu.

5. Detailed Usage with Examples
5.1 Expression Calculator (Option 1)
This is the core feature. You can enter any arithmetic expression with functions.

Commands inside the expression calculator:

hist – show calculation history

back – return to main menu

Supported operators: +, -, *, /, ^ (power), % (modulo).

Supported constants: pi (π), e (Euler’s number).

Implicit multiplication is allowed: 2pi, 3(2+4), (x+1)(x-1).

Number prefixes:

Binary: 0b1010 → 10

Octal: 0o77 → 63

Hexadecimal: 0xFF → 255

Examples (type after expr> prompt):

Input	Output
2+3*4	14
(2+3)*4	20
10/3	3.333333333
2^10	1024
2^3^2	512 (right‑associative)
sqrt(16)	4
sin(pi/2)	1
sind(90)	1
ln(e)	1
log(100)	2
log10(100)	2
log2(8)	3
exp(1)	2.718281828
abs(-3.5)	3.5
floor(3.7)	3
ceil(3.2)	4
round(3.5)	4
fact(5)	120
root(8,3)	2
logbase(100,10)	2
logbase(8,2)	3
0b1010 + 0xFF	265
toBin(10)	1010
toHex(255)	255 (decimal digits of hex)
2pi	6.283185307
3(2+4)	18
10%3	1
hist	shows history
back	exits to main menu
5.2 Equation Solver (Option 2)
Sub‑options: 1 – Quadratic, 2 – Cubic, 3 – General (Newton).

Quadratic (ax² + bx + c = 0)
Enter coefficients a b c separated by spaces.

Examples:

text
Choice: 1
a, b, c: 1 -3 2
Two real roots: 2 , 1
text
a, b, c: 1 2 5
Complex roots: -1 + 2 i , -1 - 2 i
text
a, b, c: 0 2 -8
Linear equation, root: x = 4
Cubic (ax³ + bx² + cx + d = 0) – numerical
Enter a b c d. Program finds real roots using Newton’s method with multiple starting points.

text
a, b, c, d: 1 -6 11 -6
Real root(s): 1 2 3
General f(x) = 0 (Newton‑Raphson)
You provide an expression in x and an initial guess.

text
f(x) = x^2 - 2
Initial guess: 1
Root ~ 1.414213562
5.3 Numerical Differentiation (Option 3)
Enter a function of x and a point x0. The program computes f'(x0) using central difference (h = 1e-6).

Example:

text
f(x) = x^2
x = 2
f'(2) = 4
5.4 Temperature Conversion (Option 4)
Convert between Celsius, Fahrenheit, and Kelvin. Select the conversion type, then enter the value.

text
Choice: 1
Value: 0
0 C = 32 F
5.5 Number Base Conversion (Option 5)
Convert a number from any base (2–36) to any other base. Enter the number as a string, then the source base, then the target base.

Example:

text
Number: FF
From base (2-36): 16
To base (2-36): 10
FF (base 16) = 255 (base 10)
text
Number: 1010
From base: 2
To base: 10
1010 (base 2) = 10 (base 10)
5.6 Special Functions (Option 6)
Quick calculation of single‑argument functions without writing a full expression.

text
1: sin(x)    2: cos(x)    3: tan(x)
4: sqrt(x)   5: ln(x)     6: log10(x)
7: exp(x)    8: fact(x)
Choice: 4
x = 2
Result: 1.414213562
5.7 ASCII Graph Plotter (Option 7)
Plot any function y = f(x) in the terminal using asterisks (*). The graph is scaled automatically.

Example:

text
f(x) = x + 5
xmin xmax: 0 9
Output: a rising straight line.

5.8 Matrix Operations (2×2) (Option 8)
Enter two matrices A and B as four numbers each (row1: a b, row2: c d). The program shows:

A + B

A × B

Determinant of A

Inverse of A (if non‑singular)

Example:

text
Enter matrix A (row1: a b, row2: c d):
1 2 3 4
Enter matrix B similarly:
5 6 7 8

A + B =
       6        8
      10       12

A * B =
      19       22
      43       50

Det(A) = -2
Inverse of A:
      -2        1
     1.5     -0.5
5.9 Exit (Option 9)
Displays encrypted (Base64) messages, then terminates.

text
VGhlIHN5c3RlbSBzbGVlcHMsIGJ1dCB0aGUgb2JzZXJ2ZXIgbmV2ZXIgZG9lcy4=
WW91IGp1c3QgY2xvc2VkIGEgcHJvY2Vzcy4gQnV0IHdobyBjbG9zZWQgeW91Pw==
...
6. Error Handling
Division by zero → error message

Invalid input (non‑numeric) → program clears buffer and asks again

Mismatched parentheses → error

Unknown function/identifier → error

The program never crashes – all errors are caught and reported.

7. History
Inside the expression calculator, type hist to see the last 20 calculations with results. The history is stored in memory only (not saved to disk).

8. Limitations
No symbolic mathematics (only numeric).

Cubic solver is numerical, may miss roots if function is flat.

Graph resolution is limited to 70×20 characters.

Matrix operations only for 2×2.

The Base64 exit messages are not real encryption; they only obfuscate the plaintext.

9. License
This program is free software. You may use, modify, and distribute it for any purpose. No warranty.

10. Credits
Design inspired by the philosophy of Dennis Ritchie: “Make it work, make it right, make it fast.”

Happy calculating!