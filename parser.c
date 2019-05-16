#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "numbers.h"
#include "variables.h"
#include "parser.h"

/******************************************************************************
 * Here begins the evaluator internals
 ******************************************************************************/

static char  LOOK;  // The character being analyzed
static char* INPUT; // The string being analyzed

/* Global variable store, we only have one variable scope */
static Store STORE;

bool p_parse(const char* line, void (*p_func)())
{
    char* input = strdup(line);
    INPUT = input;

    if (!INPUT)
        return false;

    LOOK  = INPUT[0];

    p_func();

    free(input);
    return true;
}

bool p_build()
{
    STORE = store_build(4);

    if (!STORE) {
        puts("Cannot allocate store");
        return 0;
    }

    return 1;
}

void p_free()
{
    store_free(STORE);
}

static bool is_digit(char);
static void match_digit();
static void match(char);

void match(char c)
{
    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }

    if (LOOK != c)
        expect_char(c);

    do {
        INPUT++;
        LOOK = INPUT[0];
    } while (LOOK == ' ');

}


bool is_digit(char c)
{
    return (c >= '0') && (c <= '9');
}

void match_digit()
{
    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }

    if (is_digit(LOOK)) {
        INPUT++;
        LOOK = *INPUT;
    }
    else expect("a digit");

    while (LOOK == ' ') {
        INPUT++;
        LOOK = INPUT[0];
    }
}

bool is_var_name_char(char c)
{
    if (c == ' ' || c == '=' || c == '\n')
        return false;
    return true;
}

bool is_add_op(char c)
{
    return ((c == '+') || (c == '-'));
}

bool is_prod_op(char c)
{
    return ((c == '*') || (c == '/'));
}

char* get_var_name()
{
    char* nend = INPUT;
    while (is_var_name_char(*nend))
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
    char* name = get_var_name();

    match('=');

    Number value = p_expression();

    Variable var = store_find(STORE, name);
    if (var != NULL) {
        var->value = value;
        free(name);
        return;
    }

    var = store_register(STORE, name, value);
    if (!var) {
        printf("Fatal Error: Couldn't assign variable");
        free(name);
        exit(1);
    }

    free(name);
}

void p_delete()
{
    char* name = get_var_name();

    if (!store_forget(STORE, name))
        puts("Unknown variable");

    free(name);
}

Number p_expression()
{
    Number result = p_term();

    while (is_add_op(LOOK)) {
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

    while (is_prod_op(LOOK)) {
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

    while (is_digit(LOOK)) {
        result = number_add(number_mul(result, number_build_int(10)),
                            number_build_int(LOOK - '0'));
        match_digit();
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
    while (is_digit(LOOK)) {
        decimal_places++;
        result = number_add(number_mul(result, number_build_int(10)),
                            number_build_int(LOOK - '0'));
        match_digit();
    }

    result = number_build_float(result.int_val);

    Number ten = number_build_int(10);
    for (size_t i=0 ; i<decimal_places ; i++)
        result = number_div(result, ten);

    return result;
}

Number p_variable()
{
    char* name = get_var_name();

    Variable var = store_find(STORE, name);
    free(name);

    if (!var) {
        puts("Unknown variable");
        return number_build_int(0);
    }

    return var->value;
}
