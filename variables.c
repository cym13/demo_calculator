#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "numbers.h"
#include "variables.h"

Variable variable_build(Store store, const char* name, const Number value)
{
    if (name[0] == '\0')
        return NULL;

    struct Variable_t result;
    strncpy(result.name, name, strlen(name)+1);
    result.value = value;

    return store_register(store, &result);
}

char* variable_repr(Variable var)
{
    size_t len = snprintf(NULL, 0, "$%s(%s)",
                          var->name, number_repr(var->value)) + 1;
    char* buf = calloc(len, sizeof(char));
    snprintf(buf, len, "$%s(%s)", var->name, number_repr(var->value));
    return buf;
}

Store store_build(size_t length)
{
    Store result = (Store) malloc(sizeof(struct Store_t));

    result->num_var = 0,
    result->length  = length,
    result->variables = (Variable) calloc(length, sizeof(struct Variable_t));

    return result;
}

bool store_expand(Store self, size_t length)
{
    if (length < self->length)
        return false;

    Variable new_buffer = (Variable) calloc(length, sizeof(struct Variable_t));
    if (new_buffer == NULL)
        return false;

    for (size_t i=0 ; i<self->num_var ; i++)
        new_buffer[i] = self->variables[i];

    free(self->variables);
    self->variables = new_buffer;
    return true;
}

void store_free(Store self)
{
    free(self->variables);
    free(self);
}

Variable store_register(Store self, const Variable variable)
{
    // First check that it doesn't already exist
    Variable existing = store_find(self, variable->name);
    if (existing) {
        existing->value = variable->value;
        return existing;
    }

    // Then check for a free slot before the end
    for (size_t i=0 ; i<self->num_var ; i++) {
        if (self->variables[i].name[0] == '\0') {
            self->variables[i] = *variable;
            return &self->variables[i];
        }
    }

    // No luck, let's put it at the end, do we need more space?
    if (self->num_var+1 > self->length)
        if (!store_expand(self, self->length * 2))
            return NULL;

    // And insert
    self->variables[self->num_var] = *variable;
    return &self->variables[self->num_var++];
}

Variable store_find(Store self, const char* name)
{
    if (name[0] == '\0')
        return NULL;

    for (size_t i=0 ; i<self->num_var ; i++)
        if (!strcmp(self->variables[i].name, name))
            return &self->variables[i];

    return NULL;
}

bool store_forget(Store self, const char* name)
{
    Variable result = store_find(self, name);

    if (result == NULL)
        return false;

    result->name[0] = '\0';
    self->num_var--;
    return true;
}

void store_print(Store self)
{
    printf("Num Var: %d\n", self->num_var);
    for (size_t i=0 ; i<self->num_var ; i++) {
        if (self->variables[i].name[0] == '\0')
            continue;
        char* repr = variable_repr(&(self->variables[i]));
        printf("%s\n", repr);
        free(repr);
    }
}
