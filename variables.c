#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "numbers.h"
#include "variables.h"

Variable variable_build(const char* name, const Number value)
{
    Variable result = (Variable)malloc(sizeof(struct Variable_t));
    strcpy(result->name, name);
    result->value = value;
    return result;
}

void variable_free(Variable v)
{
    free(v);
}
