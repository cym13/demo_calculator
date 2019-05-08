#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>
#include "numbers.h"

#define MAX_NAME_LEN 64

struct Variable_t {
    Number value;
    char   name[MAX_NAME_LEN];
};

typedef struct Variable_t* Variable;

struct Store_t {
    size_t num_var;
    size_t length;
    Variable variables;
};

typedef struct Store_t* Store;

Variable variable_build(Store store, const char* name, const Number value);
char*    variable_repr(Variable self);

Store    store_build(size_t length);
bool     store_expand(Store self, size_t length);
void     store_free(Store store);
Variable store_register(Store self, const char* name, Number value);
Variable store_find(Store self, const char* name);
bool     store_forget(Store self, const char* name);
void     store_print(Store self);

#endif
