#ifndef PARSER_H
#define PARSER_H

#define expect(msg)      do{ printf("Expecting %s.\n", msg); exit(1); }while(0)
#define expect_char(chr) do{ printf("Expecting %c.\n", chr); exit(1); }while(0)

bool p_parse(const char* line, void (*p_func)());
bool p_build();
void p_free();

void p_line();
void p_assign();
void p_delete();
Number p_expression();
Number p_add();
Number p_substract();
Number p_multiply();
Number p_divide();
Number p_factor();
Number p_term();
Number p_number();
Number p_variable();


#endif
