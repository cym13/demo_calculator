#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>
#include "numbers.h"

#define MAX_NAME_LEN 64

struct Variable_t {
    char   name[MAX_NAME_LEN];
    Number value;
};

typedef struct Variable_t* Variable;

Variable variable_build(const char* name, const Number value);
void variable_free(Variable);

#endif
