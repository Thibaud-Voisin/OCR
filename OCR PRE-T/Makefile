CC = gcc

CFLAGS = `pkg-config --cflags --libs sdl2` -Wall -Werror -g
LDFLAGS = -g

OBJ = main.o\
     pixel.o\
     grayscale.o\
     display.o\
     Binarize.o\
     tools.o\
     visuals.o

all : ${OBJ}
	gcc ${OBJ} ${CFLAGS}

clean :
	rm -rf *.o
	rm -rf *.out
