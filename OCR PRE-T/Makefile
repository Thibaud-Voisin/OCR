CC = gcc

CFLAGS = `pkg-config --cflags --libs sdl2` -Wall -Werror -g
LDFLAGS = -g

OBJ = main.o\
     pixel.o\
     display.o\
     pre_processing.o\
     tools.o\
     visuals.o\
     segmentation.o

all : ${OBJ}
	gcc ${OBJ} ${CFLAGS}

clean :
	rm -rf *.o
	rm -rf *.out
