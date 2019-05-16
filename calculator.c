#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "history.h"
#include "parser.h"
#include "numbers.h"

static char HELP_MSG[] = "Simple command-line calculator\n\
\n\
Usage: calculator [-h]\n\
\n\
Example:\n\
    [1]> help\n\
    help: print this help\n\
    let: set variable value\n\
    del: delete a variable\n\
    save_to: save history to path\n\
    [2]> 2+2\n\
    4\n\
    [3]> let i = (-3)*7.2\n\
    [4]> $i + 2\n\
    -19.599998\n\
    [5]> #2 + $i\n\
    -17.599998\n\
    [6]> del i\n\
    [7]>\n\
    (empty line exits)\n\
";


/******************************************************************************
 * User Interface
 ******************************************************************************/

int main(int argc, char *argv[])
{
    if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        printf("%s", HELP_MSG);
        return 0;
    }

    if (!p_build())
        return 1;

    if (!history_build())
        return 1;

    char*  line     = NULL;
    size_t line_len = 0;

    do {
        free(line);
        printf("[%ld]> ", history_num_entries()+1);
        getline(&line, &line_len, stdin);

        if (!line) {
            fprintf(stderr, "Allocation error\n");
            return 1;
        }

        Number result = p_parse(line, &p_line);
        if (!history_add(strdup(line), result)) {
            puts("ERROR: couldn't add command to history");
            return 1;
        }

        line_len = 0;
    } while (line[0] != '\n');

    free(line);
    history_free();
    p_free();
    return 0;
}
