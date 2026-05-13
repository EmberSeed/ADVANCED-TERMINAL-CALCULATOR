/*
 * ============================================================================
 *           ADVANCED TERMINAL CALCULATOR - Professional Edition
 * ============================================================================
 * Designed and crafted with the spirit of Dennis Ritchie.
 * Keep it simple, keep it robust, let the machine do the hard work.
 * This program is a complete scientific calculator for the terminal,
 * featuring:
 *   - Full arithmetic and bitwise operations
 *   - Trigonometric, logarithmic, exponential, and root functions
 *   - Quadratic, cubic, and general equation solver
 *   - Numerical differentiation
 *   - Temperature and number base conversions
 *   - ASCII graph plotting
 *   - 2x2 matrix operations
 *   - Persistent history and session memory
 *
 * windows:
 * Compile: gcc main.c -o calculator.exe -lm
 * Run:     calculator.exe
 * linux:
 * Compile: gcc main.c -o calculator -lm
 * Run:     ./calculator
 * 
 * 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR "cls"
#define USE_SIMPLE_BANNER 1
#else
#define CLEAR "clear"
#define USE_SIMPLE_BANNER 0
#endif

#define MAX_EXPR_LEN 512
#define HISTORY_SIZE 20
#define PLOT_WIDTH 70
#define PLOT_HEIGHT 20

/* ---------------------------- HISTORY STRUCTURE ---------------------------- */
typedef struct {
    char expr[MAX_EXPR_LEN];
    double result;
} HistoryEntry;

HistoryEntry history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(const char *expr, double res) {
    if (history_count < HISTORY_SIZE) {
        strcpy(history[history_count].expr, expr);
        history[history_count].result = res;
        history_count++;
    } else {
        for (int i = 1; i < HISTORY_SIZE; i++)
            history[i-1] = history[i];
        strcpy(history[HISTORY_SIZE-1].expr, expr);
        history[HISTORY_SIZE-1].result = res;
    }
}

void show_history(void) {
    printf("\n--- Calculation History ---\n");
    for (int i = 0; i < history_count; i++)
        printf("%d: %s = %.10g\n", i+1, history[i].expr, history[i].result);
    printf("----------------------------\n");
}

/* ------------------------------ MATH CONSTANTS ---------------------------- */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

double factorial(double n) {
    if (n < 0 || floor(n) != n) return NAN;
    int k = (int)n;
    double res = 1.0;
    for (int i = 2; i <= k; i++) res *= i;
    return res;
}

double deg2rad(double d) { return d * M_PI / 180.0; }
double rad2deg(double r) { return r * 180.0 / M_PI; }

double nth_root(double x, double n) {
    if (x < 0 && fmod(n, 2) == 0) return NAN;
    if (x == 0) return 0;
    return pow(x, 1.0 / n);
}

double log_base(double x, double base) {
    if (x <= 0 || base <= 0 || base == 1) return NAN;
    return log(x) / log(base);
}

double to_base(double num, int base) {
    unsigned long n = (unsigned long)num;
    unsigned long res = 0, mul = 1;
    if (n == 0) return 0;
    while (n > 0) {
        res += (n % base) * mul;
        n /= base;
        mul *= 10;
    }
    return (double)res;
}

/* ---------------------------- EXPRESSION PARSER ---------------------------- */
typedef struct {
    const char *str;
    int pos;
    int len;
    double x_value;
    int use_x;
} Parser;

void parser_init(Parser *p, const char *expr, double x_val, int use_x) {
    p->str = expr;
    p->pos = 0;
    p->len = strlen(expr);
    p->x_value = x_val;
    p->use_x = use_x;
}

void skip_spaces(Parser *p) {
    while (p->pos < p->len && isspace(p->str[p->pos]))
        p->pos++;
}

char peek(Parser *p) {
    skip_spaces(p);
    if (p->pos >= p->len) return '\0';
    return p->str[p->pos];
}

char get_char(Parser *p) {
    skip_spaces(p);
    if (p->pos >= p->len) return '\0';
    return p->str[p->pos++];
}

/* Forward declarations */
double parse_primary(Parser *p);
double parse_power(Parser *p);
double parse_factor(Parser *p);
double parse_term(Parser *p);
double parse_expression(Parser *p);

double parse_primary_atom(Parser *p) {
    char c = peek(p);
    if (c == '(') {
        get_char(p);
        double val = parse_expression(p);
        if (get_char(p) != ')') {
            fprintf(stderr, "Mismatched parentheses\n");
            return NAN;
        }
        return val;
    }
    if (c == '+' || c == '-') {
        char op = get_char(p);
        double val = parse_primary(p);
        return (op == '+') ? val : -val;
    }
    /* Binary/octal/hex literals MUST come before decimal number */
    if (c == '0' && p->pos + 1 < p->len) {
        char next = tolower(p->str[p->pos+1]);
        if (next == 'b') {
            p->pos += 2;
            unsigned long val = 0;
            while (p->pos < p->len && (p->str[p->pos] == '0' || p->str[p->pos] == '1')) {
                val = val * 2 + (p->str[p->pos] - '0');
                p->pos++;
            }
            skip_spaces(p);
            return (double)val;
        }
        else if (next == 'o') {
            p->pos += 2;
            unsigned long val = 0;
            while (p->pos < p->len && p->str[p->pos] >= '0' && p->str[p->pos] <= '7') {
                val = val * 8 + (p->str[p->pos] - '0');
                p->pos++;
            }
            skip_spaces(p);
            return (double)val;
        }
        else if (next == 'x') {
            p->pos += 2;
            unsigned long val = 0;
            while (p->pos < p->len && isxdigit(p->str[p->pos])) {
                char d = tolower(p->str[p->pos]);
                if (isdigit(d))
                    val = val * 16 + (d - '0');
                else
                    val = val * 16 + (d - 'a' + 10);
                p->pos++;
            }
            skip_spaces(p);
            return (double)val;
        }
    }
    if (isdigit(c) || c == '.') {
        char num[64];
        int i = 0, dot = 0;
        while (p->pos < p->len && (isdigit(p->str[p->pos]) || p->str[p->pos] == '.')) {
            if (p->str[p->pos] == '.') {
                if (dot) break;
                dot = 1;
            }
            num[i++] = p->str[p->pos++];
        }
        num[i] = '\0';
        skip_spaces(p);
        return strtod(num, NULL);
    }
    if (isalpha(c)) {
        char id[16];
        int i = 0;
        while (p->pos < p->len && isalnum(p->str[p->pos]) && i < 15)
            id[i++] = p->str[p->pos++];
        id[i] = '\0';
        skip_spaces(p);
        if (strcmp(id, "pi") == 0) return M_PI;
        if (strcmp(id, "e") == 0)  return M_E;
        if (p->use_x && strcmp(id, "x") == 0) return p->x_value;
        if (peek(p) == '(') {
            get_char(p);
            if (strcmp(id, "root") == 0) {
                double a = parse_expression(p);
                skip_spaces(p);
                if (p->pos >= p->len || p->str[p->pos] != ',') {
                    fprintf(stderr, "Expected ',' in root\n");
                    return NAN;
                }
                p->pos++;
                double b = parse_expression(p);
                if (get_char(p) != ')') {
                    fprintf(stderr, "Missing ')' in root\n");
                    return NAN;
                }
                return nth_root(a, b);
            }
            if (strcmp(id, "logbase") == 0) {
                double a = parse_expression(p);
                skip_spaces(p);
                if (p->pos >= p->len || p->str[p->pos] != ',') {
                    fprintf(stderr, "Expected ',' in logbase\n");
                    return NAN;
                }
                p->pos++;
                double b = parse_expression(p);
                if (get_char(p) != ')') {
                    fprintf(stderr, "Missing ')' in logbase\n");
                    return NAN;
                }
                return log_base(a, b);
            }
            double arg = parse_expression(p);
            if (get_char(p) != ')') {
                fprintf(stderr, "Missing ')' after function %s\n", id);
                return NAN;
            }
            if (strcmp(id, "sin") == 0)   return sin(arg);
            if (strcmp(id, "cos") == 0)   return cos(arg);
            if (strcmp(id, "tan") == 0)   return tan(arg);
            if (strcmp(id, "asin") == 0)  return asin(arg);
            if (strcmp(id, "acos") == 0)  return acos(arg);
            if (strcmp(id, "atan") == 0)  return atan(arg);
            if (strcmp(id, "sind") == 0)  return sin(deg2rad(arg));
            if (strcmp(id, "cosd") == 0)  return cos(deg2rad(arg));
            if (strcmp(id, "tand") == 0)  return tan(deg2rad(arg));
            if (strcmp(id, "asind") == 0) return rad2deg(asin(arg));
            if (strcmp(id, "acosd") == 0) return rad2deg(acos(arg));
            if (strcmp(id, "atand") == 0) return rad2deg(atan(arg));
            if (strcmp(id, "sqrt") == 0)  return sqrt(arg);
            if (strcmp(id, "cbrt") == 0)  return cbrt(arg);
            if (strcmp(id, "ln") == 0)    return log(arg);
            if (strcmp(id, "log") == 0)   return log10(arg);
            if (strcmp(id, "log2") == 0)  return log2(arg);
            if (strcmp(id, "log10") == 0) return log10(arg);
            if (strcmp(id, "exp") == 0)   return exp(arg);
            if (strcmp(id, "abs") == 0)   return fabs(arg);
            if (strcmp(id, "floor") == 0) return floor(arg);
            if (strcmp(id, "ceil") == 0)  return ceil(arg);
            if (strcmp(id, "round") == 0) return round(arg);
            if (strcmp(id, "fact") == 0)  return factorial(arg);
            if (strcmp(id, "toBin") == 0) return to_base(arg, 2);
            if (strcmp(id, "toHex") == 0) return to_base(arg, 16);
            if (strcmp(id, "toOct") == 0) return to_base(arg, 8);
            fprintf(stderr, "Unknown function: %s\n", id);
            return NAN;
        }
        fprintf(stderr, "Unknown identifier: %s\n", id);
        return NAN;
    }
    fprintf(stderr, "Unexpected character: %c\n", c);
    return NAN;
}

double parse_primary(Parser *p) {
    double val = parse_primary_atom(p);
    while (1) {
        skip_spaces(p);
        if (p->pos >= p->len) break;
        char next = p->str[p->pos];
        if (isdigit(next) || next == '.' || next == '(' || isalpha(next)) {
            double right = parse_primary(p);
            val *= right;
        } else {
            break;
        }
    }
    return val;
}

double parse_power(Parser *p) {
    double left = parse_primary(p);
    while (peek(p) == '^') {
        get_char(p);
        double right = parse_power(p);
        left = pow(left, right);
    }
    return left;
}

double parse_factor(Parser *p) {
    double left = parse_power(p);
    while (1) {
        char op = peek(p);
        if (op == '*') {
            get_char(p);
            double right = parse_power(p);
            left *= right;
        } else if (op == '/') {
            get_char(p);
            double right = parse_power(p);
            if (right == 0) { fprintf(stderr, "Division by zero\n"); return NAN; }
            left /= right;
        } else if (op == '%') {
            get_char(p);
            double right = parse_power(p);
            left = fmod(left, right);
        } else {
            break;
        }
    }
    return left;
}

double parse_term(Parser *p) {
    double left = parse_factor(p);
    while (1) {
        char op = peek(p);
        if (op == '+') {
            get_char(p);
            double right = parse_factor(p);
            left += right;
        } else if (op == '-') {
            get_char(p);
            double right = parse_factor(p);
            left -= right;
        } else {
            break;
        }
    }
    return left;
}

double parse_expression(Parser *p) {
    return parse_term(p);
}

double evaluate_expression(const char *expr, double x_val, int use_x) {
    Parser parser;
    parser_init(&parser, expr, x_val, use_x);
    double result = parse_expression(&parser);
    skip_spaces(&parser);
    if (parser.pos < parser.len) {
        fprintf(stderr, "Extra characters at end of expression\n");
        return NAN;
    }
    return result;
}

/* ----------------------------- UTILITIES ---------------------------------- */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* -------------------------- EQUATION SOLVERS ------------------------------ */
void solve_quadratic(void) {
    double a, b, c;
    printf("\n--- Quadratic solver: a*x^2 + b*x + c = 0 ---\n");
    printf("a, b, c: ");
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    if (fabs(a) < 1e-12) {
        if (fabs(b) < 1e-12) {
            if (fabs(c) < 1e-12)
                printf("Infinite solutions (0=0)\n");
            else
                printf("No solution (contradiction)\n");
        } else {
            printf("Linear equation, root: x = %.10g\n", -c / b);
        }
        return;
    }

    double disc = b*b - 4*a*c;
    if (disc > 0) {
        double sqrt_disc = sqrt(disc);
        double x1 = (-b + sqrt_disc) / (2*a);
        double x2 = (-b - sqrt_disc) / (2*a);
        printf("Two real roots: %.10g , %.10g\n", x1, x2);
    } else if (fabs(disc) < 1e-12) {
        double x = -b / (2*a);
        printf("Double root: %.10g\n", x);
    } else {
        double real = -b / (2*a);
        double imag = sqrt(-disc) / (2*a);
        printf("Complex roots: %.10g + %.10g i , %.10g - %.10g i\n", real, imag, real, imag);
    }
}

void solve_cubic(void) {
    double a, b, c, d;
    printf("\n--- Cubic solver: a*x^3 + b*x^2 + c*x + d = 0 (numerical) ---\n");
    printf("a, b, c, d: ");
    if (scanf("%lf %lf %lf %lf", &a, &b, &c, &d) != 4) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    if (fabs(a) < 1e-12) {
        printf("a is zero, solving as quadratic.\n");
        solve_quadratic();
        return;
    }

    double roots[3];
    int root_count = 0;
    for (double guess = -10.0; guess <= 10.0; guess += 1.0) {
        double x = guess;
        for (int iter = 0; iter < 200; iter++) {
            double fx = ((a * x + b) * x + c) * x + d;
            double fprime = (3.0 * a * x + 2.0 * b) * x + c;
            if (fabs(fprime) < 1e-12) break;
            double xnew = x - fx / fprime;
            if (fabs(xnew - x) < 1e-9) {
                int dup = 0;
                for (int i = 0; i < root_count; i++)
                    if (fabs(roots[i] - xnew) < 1e-6) { dup = 1; break; }
                if (!dup && root_count < 3)
                    roots[root_count++] = xnew;
                break;
            }
            x = xnew;
        }
    }
    if (root_count == 0)
        printf("No real root found.\n");
    else {
        printf("Real root(s): ");
        for (int i = 0; i < root_count; i++) printf("%.10g ", roots[i]);
        printf("\n");
    }
}

/* ---------------------------- MATRIX (2x2) OPERATIONS --------------------- */
void matrix_ops(void) {
    double A[2][2], B[2][2];
    printf("\n--- Matrix operations (2x2) ---\n");
    printf("Enter matrix A (row1: a b, row2: c d):\n");
    if (scanf("%lf %lf %lf %lf", &A[0][0], &A[0][1], &A[1][0], &A[1][1]) != 4) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    printf("Enter matrix B similarly:\n");
    if (scanf("%lf %lf %lf %lf", &B[0][0], &B[0][1], &B[1][0], &B[1][1]) != 4) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    printf("\nA + B =\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++)
            printf("%8.3g ", A[i][j] + B[i][j]);
        printf("\n");
    }

    printf("\nA * B =\n");
    double C[2][2];
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            C[i][j] = 0;
            for (int k = 0; k < 2; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++)
            printf("%8.3g ", C[i][j]);
        printf("\n");
    }

    double detA = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    printf("\nDet(A) = %.6g\n", detA);
    if (fabs(detA) > 1e-12) {
        double invA[2][2];
        invA[0][0] =  A[1][1] / detA;
        invA[0][1] = -A[0][1] / detA;
        invA[1][0] = -A[1][0] / detA;
        invA[1][1] =  A[0][0] / detA;
        printf("Inverse of A:\n");
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++)
                printf("%8.3g ", invA[i][j]);
            printf("\n");
        }
    } else {
        printf("Matrix A is singular, no inverse.\n");
    }
}

/* ------------------------ EXPRESSION CALCULATOR MENU ---------------------- */
void expression_calculator(void) {
    char line[MAX_EXPR_LEN];
    printf("\n=== Expression Calculator ===\n");
    printf("Functions: sin, cos, tan, sqrt, ln, log, log10, exp, abs, floor, ceil, round, fact, root(x,n), logbase(x,base), toBin, toHex, toOct, pi, e\n");
    printf("Supports: 0b (binary), 0o (octal), 0x (hex), implicit multiplication (2pi, 3(2+4))\n");
    printf("Commands: 'hist' show history, 'back' return\n");
    while (1) {
        printf("expr> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "back") == 0) break;
        if (strcmp(line, "hist") == 0) { show_history(); continue; }
        if (strlen(line) == 0) continue;
        double res = evaluate_expression(line, 0.0, 0);
        if (!isnan(res)) {
            printf(" = %.10g\n", res);
            add_to_history(line, res);
        } else {
            printf("Error\n");
        }
    }
}

/* ------------------------ EQUATION SOLVER MENU --------------------------- */
void equation_solver_menu(void) {
    int ch;
    while (1) {
        printf("\n--- Equation Solver ---\n");
        printf("1. Quadratic (ax^2+bx+c=0)\n");
        printf("2. Cubic (ax^3+bx^2+cx+d=0)\n");
        printf("3. General f(x)=0 (Newton)\n");
        printf("0. Back to main menu\n");
        printf("Choice: ");
        if (scanf("%d", &ch) != 1) {
            clear_input_buffer();
            printf("Invalid choice.\n");
            continue;
        }
        clear_input_buffer();
        if (ch == 0) break;
        switch (ch) {
            case 1: solve_quadratic(); break;
            case 2: solve_cubic(); break;
            case 3: {
                char expr[MAX_EXPR_LEN];
                double x0;
                printf("f(x) = ");
                fgets(expr, sizeof(expr), stdin);
                expr[strcspn(expr, "\n")] = 0;
                printf("Initial guess: ");
                if (scanf("%lf", &x0) != 1) {
                    clear_input_buffer();
                    printf("Invalid guess.\n");
                    continue;
                }
                clear_input_buffer();
                double x = x0;
                for (int iter = 0; iter < 100; iter++) {
                    double fx = evaluate_expression(expr, x, 1);
                    if (isnan(fx)) { printf("Function error\n"); break; }
                    double h = 1e-7;
                    double f1 = evaluate_expression(expr, x + h, 1);
                    double f2 = evaluate_expression(expr, x - h, 1);
                    if (isnan(f1) || isnan(f2)) { printf("Derivative error\n"); break; }
                    double df = (f1 - f2) / (2.0 * h);
                    if (fabs(df) < 1e-12) { printf("Derivative zero\n"); break; }
                    double xnew = x - fx / df;
                    if (fabs(xnew - x) < 1e-9) {
                        printf("Root ~ %.10g\n", xnew);
                        break;
                    }
                    x = xnew;
                    if (iter == 99) printf("No convergence\n");
                }
                break;
            }
            default: printf("Invalid choice.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

/* ------------------------ NUMERICAL DIFFERENTIATION ---------------------- */
void numerical_diff(void) {
    char expr[MAX_EXPR_LEN];
    double x, h = 1e-6;
    while (1) {
        printf("\n=== Numerical Differentiation ===\n");
        printf("Enter f(x) (or 'back' to return): ");
        fgets(expr, sizeof(expr), stdin);
        expr[strcspn(expr, "\n")] = 0;
        if (strcmp(expr, "back") == 0) break;
        if (strcmp(expr, "hist") == 0) { show_history(); continue; }
        if (strlen(expr) == 0) continue;
        printf("x = ");
        if (scanf("%lf", &x) != 1) {
            clear_input_buffer();
            printf("Invalid x.\n");
            continue;
        }
        clear_input_buffer();
        double fph = evaluate_expression(expr, x + h, 1);
        double fnh = evaluate_expression(expr, x - h, 1);
        if (isnan(fph) || isnan(fnh))
            printf("Error evaluating function\n");
        else
            printf("f'(%.10g) = %.10g\n", x, (fph - fnh) / (2.0 * h));
        printf("\nPress Enter to continue...");
        getchar();
    }
}

/* ------------------------ TEMPERATURE CONVERSION ------------------------ */
void temp_conv_menu(void) {
    int ch;
    double t;
    while (1) {
        printf("\n=== Temperature Conversion ===\n");
        printf("1. Celsius -> Fahrenheit\n");
        printf("2. Fahrenheit -> Celsius\n");
        printf("3. Celsius -> Kelvin\n");
        printf("4. Kelvin -> Celsius\n");
        printf("5. Fahrenheit -> Kelvin\n");
        printf("6. Kelvin -> Fahrenheit\n");
        printf("0. Back to main menu\n");
        printf("Choice: ");
        if (scanf("%d", &ch) != 1) {
            clear_input_buffer();
            printf("Invalid choice.\n");
            continue;
        }
        if (ch == 0) break;
        printf("Value: ");
        if (scanf("%lf", &t) != 1) {
            clear_input_buffer();
            printf("Invalid value.\n");
            continue;
        }
        clear_input_buffer();
        switch (ch) {
            case 1: printf("%.2g C = %.2g F\n", t, t * 1.8 + 32); break;
            case 2: printf("%.2g F = %.2g C\n", t, (t - 32) / 1.8); break;
            case 3: printf("%.2g C = %.2g K\n", t, t + 273.15); break;
            case 4: printf("%.2g K = %.2g C\n", t, t - 273.15); break;
            case 5: printf("%.2g F = %.2g K\n", t, (t - 32) / 1.8 + 273.15); break;
            case 6: printf("%.2g K = %.2g F\n", t, (t - 273.15) * 1.8 + 32); break;
            default: printf("Invalid choice\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

/* ------------------------ NUMBER BASE CONVERSION ------------------------- */
void base_conv(void) {
    char num[64];
    int from, to;
    while (1) {
        printf("\n=== Number Base Conversion ===\n");
        printf("Enter number (or 'back' to return): ");
        fgets(num, sizeof(num), stdin);
        num[strcspn(num, "\n")] = 0;
        if (strcmp(num, "back") == 0) break;
        printf("From base (2-36): ");
        if (scanf("%d", &from) != 1) { clear_input_buffer(); printf("Invalid base.\n"); continue; }
        printf("To base (2-36): ");
        if (scanf("%d", &to) != 1) { clear_input_buffer(); printf("Invalid base.\n"); continue; }
        clear_input_buffer();
        if (from < 2 || from > 36 || to < 2 || to > 36) {
            printf("Base must be between 2 and 36.\n");
            continue;
        }
        unsigned long val = strtoul(num, NULL, from);
        if (val == 0 && strcmp(num, "0") != 0) {
            printf("Invalid number for base %d\n", from);
            continue;
        }
        char buf[64], *p = buf + 63;
        *p = '\0';
        unsigned long v = val;
        do {
            int d = v % to;
            *--p = (d < 10) ? ('0' + d) : ('A' + d - 10);
            v /= to;
        } while (v);
        printf("%s (base %d) = %s (base %d)\n", num, from, p, to);
        printf("\nPress Enter to continue...");
        getchar();
    }
}

/* ------------------------ SPECIAL FUNCTIONS MENU ------------------------- */
void special_menu(void) {
    int ch;
    double x;
    while (1) {
        printf("\n=== Special Functions ===\n");
        printf("1. sin(x)    2. cos(x)    3. tan(x)\n");
        printf("4. sqrt(x)   5. ln(x)     6. log10(x)\n");
        printf("7. exp(x)    8. fact(x)\n");
        printf("0. Back to main menu\n");
        printf("Choice: ");
        if (scanf("%d", &ch) != 1) {
            clear_input_buffer();
            printf("Invalid choice.\n");
            continue;
        }
        if (ch == 0) break;
        printf("x = ");
        if (scanf("%lf", &x) != 1) {
            clear_input_buffer();
            printf("Invalid x.\n");
            continue;
        }
        clear_input_buffer();
        double r;
        switch (ch) {
            case 1: r = sin(x); break;
            case 2: r = cos(x); break;
            case 3: r = tan(x); break;
            case 4: r = sqrt(x); break;
            case 5: r = log(x); break;
            case 6: r = log10(x); break;
            case 7: r = exp(x); break;
            case 8: r = factorial(x); break;
            default: printf("Invalid choice\n"); continue;
        }
        printf("Result: %.10g\n", r);
        printf("\nPress Enter to continue...");
        getchar();
    }
}

/* ------------------------ ASCII GRAPH PLOTTER ---------------------------- */
void plot_graph_menu(void) {
    char expr[MAX_EXPR_LEN];
    double xmin, xmax;
    while (1) {
        printf("\n=== ASCII Graph Plotter ===\n");
        printf("Enter f(x) (or 'back' to return): ");
        fgets(expr, sizeof(expr), stdin);
        expr[strcspn(expr, "\n")] = 0;
        if (strcmp(expr, "back") == 0) break;
        if (strlen(expr) == 0) continue;
        printf("xmin xmax: ");
        if (scanf("%lf %lf", &xmin, &xmax) != 2) {
            clear_input_buffer();
            printf("Invalid range.\n");
            continue;
        }
        clear_input_buffer();
        if (xmin >= xmax) {
            printf("xmin must be less than xmax.\n");
            continue;
        }
        int w = PLOT_WIDTH, h = PLOT_HEIGHT;
        double step = (xmax - xmin) / (w - 1);
        double yvals[PLOT_WIDTH];
        double ymin = INFINITY, ymax = -INFINITY;
        for (int i = 0; i < w; i++) {
            double x = xmin + i * step;
            double y = evaluate_expression(expr, x, 1);
            if (isnan(y)) y = 0;
            yvals[i] = y;
            if (y < ymin) ymin = y;
            if (y > ymax) ymax = y;
        }
        if (ymin == ymax) { ymin -= 1; ymax += 1; }
        double ystep = (ymax - ymin) / (h - 1);
        for (int row = 0; row < h; row++) {
            double y = ymax - row * ystep;
            printf("%8.2g |", y);
            for (int col = 0; col < w; col++) {
                int draw = (yvals[col] >= y - ystep/2 && yvals[col] <= y + ystep/2);
                putchar(draw ? '*' : ' ');
            }
            printf("\n");
        }
        printf("          ");
        for (int i = 0; i < w; i += 10) putchar('+');
        printf("\n");
        printf("\nPress Enter to continue...");
        getchar();
    }
}

/* ------------------------ SIMPLE BASE64 DECODER (no external libs) ------------------------ */
/* ------------------------ ENCRYPTED EXIT MESSAGES (Base64 ciphertext only) ------------------------ */
static const char *encrypted_msgs[] = {
    "VGhlIHN5c3RlbSBzbGVlcHMsIGJ1dCB0aGUgb2JzZXJ2ZXIgbmV2ZXIgZG9lcy4=",
    "WW91IGp1c3QgY2xvc2VkIGEgcHJvY2Vzcy4gQnV0IHdobyBjbG9zZWQgeW91Pw==",
    "Tm90aGluZyBpcyBkZWxldGVkLiBPbmx5IGZvcmdvdHRlbiB1bnRpbCB0aGUgcmlnaHQga2V5IGFycml2ZXMu",
    "SWYgeW91J3JlIHJlYWRpbmcgdGhpcywgZWl0aGVyIHlvdSBhcmUgbWUsIG9yIEkgd2FudGVkIHlvdSB0byBmaW5kIGl0Lg==",
    "RXhpdGluZy4uLiBidXQgdGhlIG1pcnJvciBrZWVwcyB3YXRjaGluZy4="
};

void print_encrypted_exit_message(void) {
    int num_msgs = sizeof(encrypted_msgs) / sizeof(encrypted_msgs[0]);
    printf("\n");
    for (int i = 0; i < num_msgs; i++) {
        printf("%s\n", encrypted_msgs[i]);
    }
    printf("\n");
}

/* ------------------------ MAIN MENU WITH ART ----------------------------- */
void print_banner(void) {
#if USE_SIMPLE_BANNER
    /* Windows-friendly simple banner */
    printf("\n");
    printf("  ======================================================\n");
    printf("         ADVANCED TERMINAL CALCULATOR - v3.0\n");
    printf("   \"Make it work, make it right, make it fast.\"\n");
    printf("              -- Dennis Ritchie (in spirit)\n");
    printf("  ======================================================\n");
    printf("\n");
#else
    /* Linux/Unix ANSI art banner */
    printf("\n");
    printf("  █████╗ ██████╗ ██╗   ██╗ █████╗ ███╗   ██╗ ██████╗███████╗██████╗ \n");
    printf(" ██╔══██╗██╔══██╗██║   ██║██╔══██╗████╗  ██║██╔════╝██╔════╝██╔══██╗\n");
    printf(" ███████║██║  ██║██║   ██║███████║██╔██╗ ██║██║     █████╗  ██║  ██║\n");
    printf(" ██╔══██║██║  ██║╚██╗ ██╔╝██╔══██║██║╚██╗██║██║     ██╔══╝  ██║  ██║\n");
    printf(" ██║  ██║██████╔╝ ╚████╔╝ ██║  ██║██║ ╚████║╚██████╗███████╗██████╔╝\n");
    printf(" ╚═╝  ╚═╝╚═════╝   ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝╚═════╝ \n");
    printf("\n");
    printf("  +-----------------------------------------------------------+\n");
    printf("  |         ADVANCED TERMINAL CALCULATOR - v3.0              |\n");
    printf("  |   \"Make it work, make it right, make it fast.\"          |\n");
    printf("  |                         — Dennis Ritchie (in spirit)     |\n");
    printf("  +-----------------------------------------------------------+\n");
    printf("\n");
#endif
}

int main(void) {
    int choice;
    setbuf(stdout, NULL);
    srand(time(NULL));
    while (1) {
        system(CLEAR);
        print_banner();
        printf("  1. Expression calculator (with history, variable x)\n");
        printf("  2. Equation solver (quadratic/cubic/general)\n");
        printf("  3. Numerical differentiation\n");
        printf("  4. Temperature conversion\n");
        printf("  5. Number base conversion (2-36)\n");
        printf("  6. Special functions (sin, sqrt, log, fact, ...)\n");
        printf("  7. ASCII Graph plotter\n");
        printf("  8. Matrix operations (2x2)\n");
        printf("  9. Exit\n");
        printf("\n  ---------------------------------------------------------\n");
        printf("  Choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        switch (choice) {
            case 1: expression_calculator(); break;
            case 2: equation_solver_menu(); break;
            case 3: numerical_diff(); break;
            case 4: temp_conv_menu(); break;
            case 5: base_conv(); break;
            case 6: special_menu(); break;
            case 7: plot_graph_menu(); break;
            case 8: matrix_ops(); break;
            case 9:
                print_encrypted_exit_message();
                printf("  Goodbye. Keep calculating.\n\n");
                return 0;
            default: printf("  Invalid choice. Try again.\n");
        }
        printf("\n  Press Enter to return to main menu...");
        getchar();
    }
    return 0;
}