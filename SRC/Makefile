CC = gcc

CFLAGS = `pkg-config --cflags --libs sdl2` -Wall -Werror -Wextra -g -pedantic-errors -O2 -lm
LDFLAGS = -g

OBJ = main.o\
     pixel.o\
     display.o\
     pre_processing.o\
     tools.o\
     segmentation.o

NN = main_NN.o\
	 tools.o\
	 neural_network.o

pre : ${OBJ}
	gcc ${OBJ} ${CFLAGS} -o pre.out

neural : ${NN}
	gcc ${NN} ${CFLAGS} -o neural.out

clean :
	rm -rf *.o
	rm -rf *.out
