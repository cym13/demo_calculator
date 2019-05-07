#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "numbers.h"

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
    if (LOOK != c)
        expectChar(c);

    INPUT++;
    LOOK = INPUT[0];
}


bool isDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

void matchDigit()
{
    if (isDigit(LOOK)) {
        INPUT++;
        LOOK = *INPUT;
    }
    else expect("a digit");
}

/******************************************************************************
 * Here begins the parsing rules
 ******************************************************************************/

Number expression();
Number add();
Number substract();
Number multiply();
Number divide();
Number factor();
Number term();
Number number();
bool isAddOp(char);
bool isProdOp(char);


/* This is our most general expression.
 * It represents either a single element or sum of elements.
 */
Number expression()
{
    Number result = term();

    while (isAddOp(LOOK)) {
        switch (LOOK) {
            case '+': result = number_add(result, add());
                      break;
            case '-': result = number_sub(result, substract());
                      break;
        }
    }

    return result;
}

Number add()
{
    match('+');
    return term();
}

Number substract()
{
    match('-');
    return term();
}

Number multiply()
{
    match('*');
    return factor();
}

Number divide()
{
    match('/');
    Number result = factor();

    if (number_eq(result, number_build_int(0)))
        expect("non zero dividend");

    return result;
}

Number factor()
{
    Number result = number_build_int(0);

    if (LOOK == '(') {
        match('(');
        result = number_add(result, expression());
        match(')');
    }
    else {
        result = number();
    }

    return result;
}

Number term()
{
    Number result = factor();

    while (isProdOp(LOOK)) {
        switch (LOOK) {
            case '*': result = number_mul(result, multiply());
                      break;
            case '/': result = number_div(result, divide());
                      break;
        }
    }

    return result;
}

Number number()
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

bool isAddOp(char c) {
    return ((c == '+') || (c == '-'));
}

bool isProdOp(char c) {
    return ((c == '*') || (c == '/'));
}

/******************************************************************************
 * User Interface
 ******************************************************************************/

int main(int argc, char *argv[])
{
    if (argc < 2) {
        expect("a command-line argument to evaluate");
        return 1;
    }

    INPUT = argv[1];
    LOOK  = INPUT[0];
    printf("%s\n", number_repr(expression()));

    return 0;
}
