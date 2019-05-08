#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "numbers.h"
#include "variables.h"

static char HELP_MSG[] = "Simple command-line calculator\n\
\n\
Usage: calculator [-h]\n\
\n\
Example:\n\
    > 2+2\n\
    4\n\
    > let i = (-3)*7.2\n\
    $i(-21.599998)\n\
    > $i + 2\n\
    -19.599998\n\
    > del i\n\
    >\n\
    (empty line exits)\n\
";


/* Global variable store, we only have one variable scope */
static Store STORE;

/******************************************************************************
 * Here begins the evaluator internals
 ******************************************************************************/

static char  LOOK;  // The character being analyzed
static char* INPUT; // The string being analyzed

bool isDigit(char);
void matchDigit();
void match(char);

#define expect(msg)     do{ printf("Expecting %s.\n", msg); exit(1); }while(0)
#define expectChar(chr) do{ printf("Expecting %c.\n", chr); exit(1); }while(0)


void match(char c)
{
    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }

    if (LOOK != c)
        expectChar(c);

    do {
        INPUT++;
        LOOK = INPUT[0];
    } while (LOOK == ' ');

}


bool isDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

void matchDigit()
{
    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }

    if (isDigit(LOOK)) {
        INPUT++;
        LOOK = *INPUT;
    }
    else expect("a digit");

    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }
}

bool isVarNameChar(char c)
{
    if (('a' <= c) && (c <= 'z'))
        return true;

    if (('A' <= c) && (c <= 'Z'))
        return true;

    if (('0' <= c) && (c <= '9'))
        return true;

    if ((c == '_') || (c == '-') || (c == '.'))
        return true;

    return false;
}

bool isAddOp(char c)
{
    return ((c == '+') || (c == '-'));
}

bool isProdOp(char c)
{
    return ((c == '*') || (c == '/'));
}

char* getVarName()
{
    char* nend = INPUT;
    while (isVarNameChar(*nend))
        nend++;

    char* result = strndup(INPUT, nend-INPUT);

    INPUT = nend;
    LOOK  = INPUT[0];

    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }

    return result;
}

/******************************************************************************
 * Here begins the parsing rules
 ******************************************************************************/

void p_line();
void p_assign();
void p_delete();
Number p_expression();
Number p_add();
Number p_substract();
Number p_multiply();
Number p_divide();
Number p_factor();
Number p_term();
Number p_number();
Number p_variable();

void p_line()
{
    if (!strncmp(INPUT, "let ", 4)) {
        INPUT += 4;
        p_assign();
    }
    else if (!strncmp(INPUT, "del ", 4)) {
        INPUT += 4;
        p_delete();
    }
    else {
        printf("%s\n", number_repr(p_expression()));
    }
}

void p_assign()
{
    char* name = getVarName();

    match('=');

    Number value = p_expression();

    Variable var = variable_build(STORE, name, value);
    if (var == NULL) {
        printf("Fatal Error: Couldn't assign variable");
        free(name);
        exit(1);
    }

    char* repr = variable_repr(var);
    printf("%s\n", repr);
    free(repr);

    free(name);
}

void p_delete()
{
    char* name = getVarName();

    if (!store_forget(STORE, name))
        printf("Unknown variable\n");

    free(name);
}

Number p_expression()
{
    Number result = p_term();

    while (isAddOp(LOOK)) {
        switch (LOOK) {
            case '+': result = number_add(result, p_add());
                      break;
            case '-': result = number_sub(result, p_substract());
                      break;
        }
    }

    return result;
}

Number p_add()
{
    match('+');
    return p_term();
}

Number p_substract()
{
    match('-');
    return p_term();
}

Number p_multiply()
{
    match('*');
    return p_factor();
}

Number p_divide()
{
    match('/');
    Number result = p_factor();

    if (number_eq(result, number_build_int(0)))
        expect("non zero dividend");

    return result;
}

Number p_factor()
{
    Number result = number_build_int(0);

    if (LOOK == '(') {
        match('(');
        result = number_add(result, p_expression());
        match(')');
    }
    else if (LOOK == '$') {
        match('$');
        result = p_variable();
    }
    else {
        result = p_number();
    }

    return result;
}

Number p_term()
{
    Number result = p_factor();

    while (isProdOp(LOOK)) {
        switch (LOOK) {
            case '*': result = number_mul(result, p_multiply());
                      break;
            case '/': result = number_div(result, p_divide());
                      break;
        }
    }

    return result;
}

Number p_number()
{
    Number result = number_build_int(0);
    Number sign   = number_build_int(1);

    if (LOOK == '-') {
        match('-');
        sign = number_build_int(-1);
    }

    while (isDigit(LOOK)) {
        result = number_add(number_mul(result, number_build_int(10)),
                            number_build_int(LOOK - '0'));
        matchDigit();
    }

    if (LOOK != '.')
        return number_mul(result, sign);

    /* If dot is found that's a decimal number
     * In that case we want to continue growing our initial number but count
     * the number of decimal places to scale it down when converting to
     * float.
     *
     * This sadly means only smallish floating point numbers will be
     * supported.
     */

    match('.');

    size_t decimal_places = 0;
    while (isDigit(LOOK)) {
        decimal_places++;
        result = number_add(number_mul(result, number_build_int(10)),
                            number_build_int(LOOK - '0'));
        matchDigit();
    }

    result = number_build_float(result.int_val);

    Number ten = number_build_int(10);
    for (size_t i=0 ; i<decimal_places ; i++)
        result = number_div(result, ten);

    return result;
}

Number p_variable()
{
    char* name = getVarName();

    Variable var = store_find(STORE, name);
    if (var == NULL) {
        free(name);
        expect("an existing variable's name");
    }

    free(name);
    return var->value;
}

/******************************************************************************
 * User Interface
 ******************************************************************************/

int main(int argc, char *argv[])
{
    if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        printf("%s", HELP_MSG);
        return 0;
    }

    STORE = store_build(4);

    char*  line     = NULL;
    size_t line_len = 0;

    do {
        free(line);
        printf("> ");
        getline(&line, &line_len, stdin);

        if (line == NULL) {
            fprintf(stderr, "Allocation error\n");
            return 1;
        }

        INPUT = line;
        LOOK  = INPUT[0];

        p_line();
        line_len = 0;
    } while (line[0] != '\n');

    store_free(STORE);
    return 0;
}
