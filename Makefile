## This is a make file for Sudoku on linux
CFLAGS = -I../include -pthread -O2 -Wall
LFLAGS = ../lib/x11/libglfw.a -lrt -lX11 -lGLU -lGL -pthread -lm -lXrandr

all: Sudoku

Sudoku: Sudoku.c ../lib/x11/libglfw.a
	gcc $(CFLAGS) Sudoku.c $(LFLAGS) -o Sudoku

clean:
	rm -f Sudoku
