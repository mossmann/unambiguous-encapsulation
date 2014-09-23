
all: icblbc lcbbc lcbbc_flags

icblbc: icblbc.c
	gcc -o icblbc icblbc.c

lcbbc: lcbbc.c
	gcc -mpopcnt -O3 -o lcbbc lcbbc.c

lcbbc_flags: lcbbc_flags.c
	gcc -mpopcnt -O3 -o lcbbc_flags lcbbc_flags.c

clean:
	rm -f icblbc lcbbc lcbbc_flags

.PHONY: all clean
