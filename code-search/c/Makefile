
all: icblbc lcbbc lcbbc_flags icnbc

icblbc: icblbc.c
	gcc -o icblbc icblbc.c

lcbbc: lcbbc.c
	gcc -mpopcnt -O3 -o lcbbc lcbbc.c

lcbbc_flags: lcbbc_flags.c
	gcc -mpopcnt -O3 -o lcbbc_flags lcbbc_flags.c

icnbc: icnbc.c
	gcc -g -O3 -o icnbc icnbc.c -lm

clean:
	rm -f icblbc lcbbc lcbbc_flags

.PHONY: all clean
