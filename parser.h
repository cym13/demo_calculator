#ifndef PARSER_H
#define PARSER_H

#include "variables.h"

#define expect(msg)      do{ printf("Expecting %s.\n", msg); exit(1); }while(0)
#define expect_char(chr) do{ printf("Expecting %c.\n", chr); exit(1); }while(0)

bool p_build();
void p_free();

Number p_parse(const char* line, Number (*p_func)());

Number p_line();
Number p_assign();
Number p_delete();
Number p_expression();
Number p_add();
Number p_substract();
Number p_multiply();
Number p_divide();
Number p_factor();
Number p_term();
Number p_number();
Number p_variable();
Number p_help();
Number p_save();


#endif
