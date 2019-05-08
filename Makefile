
PROG := main
CC := gcc
CFLAGS := -m32


calculator: calculator.c numbers.c variables.c
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm -f calculator
