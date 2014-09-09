
all: icblbc lcbbc

icblbc: icblbc.c
	gcc -o icblbc icblbc.c

lcbbc: lcbbc.c
	gcc -mpopcnt -o lcbbc lcbbc.c

clean:
	rm -f icblbc lcbbc

.PHONY: all clean
