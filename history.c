#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"
#include "numbers.h"

static History HISTORY;

bool history_build()
{
    History hist = { }; // set all fields to zero
    HISTORY = hist;
    return true;
}

bool history_add(char* input, Number result)
{
    HistNode new = (HistNode) malloc(sizeof(struct HistNode_t));

    if (!new)
        return false;

    new->input  = input;
    new->result = result;
    new->next   = NULL;

    if (!HISTORY.first)
        HISTORY.first = new;

    if (HISTORY.last)
        HISTORY.last->next = new;

    HISTORY.last = new;
    HISTORY.num_entries++;

    return true;
}

HistNode history_get(size_t number)
{
    if (number > HISTORY.num_entries)
        return NULL;

    HistNode current = HISTORY.first;
    // Start at 1 since HistNode ids start at 1 as well
    for (size_t i=1 ; i<number ; i++)
        current = current->next;

    return current;
}

void history_free()
{
    HistNode current = HISTORY.first;

    HistNode tmp;
    while (current != NULL) {
        tmp = current->next;
        free(current->input);
        free(current);
        current = tmp;
    }
}

int history_save_to(char* path)
{
    FILE* f = fopen(path, "w");
    if (!f) {
        puts("ERROR: could not open file");
        return -1;
    }

    HistNode current = HISTORY.first;

    long result = 0;
    size_t num_input = 0;
    while (current != NULL) {
        char* repr = number_repr(current->result);

        // No \n between input and result since the input contains \n
        long written = fprintf(f, "[%d]> %s%s\n",
                               num_input++, current->input, repr);
        free(repr);

        if (written < 0) {
            puts("ERROR: could not write to file");
            return -1;
        }
        result += written;
        current = current->next;
    }

    fclose(f);
    return result;
}

size_t history_num_entries() {
    return HISTORY.num_entries;
}

