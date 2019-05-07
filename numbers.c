#include <stdio.h>
#include <stdlib.h>
#include "numbers.h"

Number __num_int_add(Number*, Number*);
Number __num_int_sub(Number*, Number*);
Number __num_int_mul(Number*, Number*);
Number __num_int_div(Number*, Number*);

bool __num_int_eq(Number*, Number*);
const char* __num_int_repr(Number*);

Number number_build_int(int val)
{
    Number n = {
        .type    = NT_INT,
        .int_val = val,
        .add     = &__num_int_add,
        .sub     = &__num_int_sub,
        .mul     = &__num_int_mul,
        .div     = &__num_int_div,
        .repr    = &__num_int_repr,
        .eq      = &__num_int_eq
    };
    return n;
}

Number __num_int_add(Number* self, Number* other)
{
    Number result;
    int current = self->int_val;

    switch (other->type) {
        case NT_INT:
            result = number_build_int(current);
            result.int_val += other->int_val;
            break;

        case NT_FLOAT:
            result = number_build_float((float)current);
            result.float_val += other->float_val;
            break;
    }
    return result;
}

Number __num_int_sub(Number* self, Number* other)
{
    Number result;
    int current = self->int_val;

    switch (other->type) {
        case NT_INT:
            result = number_build_int(current);
            result.int_val -= other->int_val;
            break;

        case NT_FLOAT:
            result = number_build_float((float)current);
            result.float_val -= other->float_val;
            break;
    }
    return result;
}

Number __num_int_mul(Number* self, Number* other)
{
    Number result;
    int current = self->int_val;

    switch (other->type) {
        case NT_INT:
            result = number_build_int(current);
            result.int_val *= other->int_val;
            break;

        case NT_FLOAT:
            result = number_build_float((float)current);
            result.float_val *= other->float_val;
            break;
    }
    return result;
}

Number __num_int_div(Number* self, Number* other)
{
    Number result;
    int current = self->int_val;

    switch (other->type) {
        case NT_INT:
            result = number_build_int(current);
            result.int_val /= other->int_val;
            break;

        case NT_FLOAT:
            result = number_build_float((float)current);
            result.float_val /= other->float_val;
            break;
    }
    return result;
}

bool __num_int_eq(Number* self, Number* other)
{
    switch (other->type) {
        case NT_INT:
            return self->int_val == other->int_val;

        case NT_FLOAT:
            if (other->float_val == 0)
                return self->float_val == 0;

            double diff = (float)self->int_val - other->float_val;
            return diff < other->float_val/100000.0f;
    }
    return false;
}

const char* __num_int_repr(Number* self)
{
    size_t repr_length = snprintf(NULL, 0, "%d", self->int_val) + 1;
    char* result = (char*)calloc(repr_length, sizeof(char));
    snprintf(result, repr_length, "%d", self->int_val);

    return (const char*)result;
}

Number __num_float_add(Number*, Number*);
Number __num_float_sub(Number*, Number*);
Number __num_float_mul(Number*, Number*);
Number __num_float_div(Number*, Number*);

bool __num_float_eq(Number* self, Number* other);
const char* __num_float_repr(Number*);

Number number_build_float(float val)
{
    Number n = {
        .type      = NT_FLOAT,
        .float_val = val,
        .add     = &__num_float_add,
        .sub     = &__num_float_sub,
        .mul     = &__num_float_mul,
        .div     = &__num_float_div,
        .repr    = &__num_float_repr,
        .eq      = &__num_float_eq
    };
    return n;
}

Number __num_float_add(Number* self, Number* other)
{
    float current = self->float_val;
    Number result = number_build_float(current);

    switch (other->type) {
        case NT_INT:
            result.float_val += (float)other->int_val;
            break;

        case NT_FLOAT:
            result.float_val += other->float_val;
            break;
    }
    return result;
}

Number __num_float_sub(Number* self, Number* other)
{
    float current = self->float_val;
    Number result = number_build_float(current);

    switch (other->type) {
        case NT_INT:
            result.float_val -= (float)other->int_val;
            break;

        case NT_FLOAT:
            result.float_val -= other->float_val;
            break;
    }
    return result;
}

Number __num_float_mul(Number* self, Number* other)
{
    float current = self->float_val;
    Number result = number_build_float(current);

    switch (other->type) {
        case NT_INT:
            result.float_val *= (float)other->int_val;
            break;

        case NT_FLOAT:
            result.float_val *= other->float_val;
            break;
    }
    return result;
}

Number __num_float_div(Number* self, Number* other)
{
    float current = self->float_val;
    Number result = number_build_float(current);

    switch (other->type) {
        case NT_INT:
            result.float_val /= (float)other->int_val;
            break;

        case NT_FLOAT:
            result.float_val /= other->float_val;
            break;
    }
    return result;
}

const char* __num_float_repr(Number* self)
{
    size_t repr_length = snprintf(NULL, 0, "%f", self->float_val) + 1;
    char* result = (char*)calloc(repr_length, sizeof(char));
    snprintf(result, repr_length, "%f", self->float_val);

    return (const char*)result;
}

bool __num_float_eq(Number* self, Number* other)
{
    double diff;
    switch (other->type) {
        case NT_INT:
            if (other->int_val == 0)
                return self->float_val == 0;

            diff = self->float_val - (float)other->int_val;
            return diff <= self->float_val/100000.0f;

        case NT_FLOAT:
            if (other->float_val == 0)
                return self->float_val == 0;

            diff = self->float_val - other->float_val;
            return diff <= self->float_val/100000.0f;
    }
    return false;
}

Number number_add(Number a, Number b)
{
    return a.add(&a, &b);
}

Number number_sub(Number a, Number b)
{
    return a.sub(&a, &b);
}

Number number_mul(Number a, Number b)
{
    return a.mul(&a, &b);
}

Number number_div(Number a, Number b)
{
    return a.div(&a, &b);
}

const char* number_repr(Number a)
{
    return a.repr(&a);
}

bool number_eq(Number a, Number b)
{
    return a.eq(&a, &b);
}
