# A Makefile that has compiles the project
# To do so - use `make`

# If no gcc change next line to `clang`
CC=gcc
CFLAGS=-Wall -Wextra -pedantic
DEBUG=-g
SRC=dissassembler.c
OUT=disassembler

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(DEBUG)

clean:
	rm $(OUT)

run: all
	./$(OUT)

