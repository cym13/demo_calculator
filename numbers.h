#ifndef NUMBERS_H
#define NUMBERS_H

#include <stdbool.h>

typedef enum {
    NT_NONE,
    NT_INT,
    NT_FLOAT
} NumType;

typedef struct Number_t {
    NumType type;
    union {
        int   int_val;
        float float_val;
    };

    struct Number_t (*add)(struct Number_t*, struct Number_t*);
    struct Number_t (*sub)(struct Number_t*, struct Number_t*);
    struct Number_t (*mul)(struct Number_t*, struct Number_t*);
    struct Number_t (*div)(struct Number_t*, struct Number_t*);

    bool (*eq)(struct Number_t*, struct Number_t*);
    const char* (*repr)(struct Number_t*);
} Number;

/*
 * Constructors
 */
Number number_build_int(int);
Number number_build_float(float);

/*
 * Arithmetics operations
 */
Number number_add(Number, Number);
Number number_sub(Number, Number);
Number number_mul(Number, Number);
Number number_div(Number, Number);

/*
 * Comparisons
 */
bool number_eq(Number, Number);

/*
 * Printable representation of the value, must be freed
 */
const char* number_repr(Number);

/*
 * Variable management
 */
bool is_var(Number);

#endif
