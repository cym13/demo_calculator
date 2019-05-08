
PROG := main
CC := gcc
CFLAGS := -m32 -g


calculator: calculator.c numbers.c variables.c
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm -f calculator
