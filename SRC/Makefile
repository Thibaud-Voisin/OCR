CC = gcc

LDFLAGS = -g

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

CFLAGS = `pkg-config --cflags --libs sdl2` ${GTKLIB} -pipe -pthread -Wall -Werror -Wextra -g -pedantic-errors -O2 -lm -rdynamic

OBJ = main_pre.o\
     pixel.o\
     display.o\
     pre_processing.o\
     tools.o\
     segmentation.o

NN = main_NN.o\
	 tools.o\
	 neural_network.o

GT = main.o\
     pixel.o\
     display.o\
     pre_processing.o\
     tools.o\
     segmentation.o

pre : ${OBJ}
	gcc ${OBJ} ${CFLAGS} -o pre.out

gtk : ${GT}
	gcc ${GT} ${CFLAGS} -o gtk.out

neural : ${NN}
	gcc ${NN} ${CFLAGS} -o neural.out

clean :
	rm -rf *.o
	rm -rf *.out
