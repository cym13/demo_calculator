#ifndef HISTORY_H
#define HISTORY_H

#include "numbers.h"

struct HistNode_t {
    char*  input;
    Number result;

    struct HistNode_t* next;
};

typedef struct HistNode_t* HistNode;

typedef struct {
    HistNode first;
    HistNode last;
    size_t   num_entries;
} History;

bool     history_build();
bool     history_add(char* input, Number result);
HistNode history_get(size_t number);
void     history_free();
size_t   history_num_entries();
int      history_save_to(char* path);

#endif
