CC = gcc

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

CFLAGS = `pkg-config --cflags --libs sdl2` ${GTKLIB} -pipe -pthread -Wall -Werror -Wextra -pedantic-errors -O2 -lm -rdynamic

OBJ = main.o\
     pixel.o\
     display.o\
     pre_processing.o\
     tools.o\
     segmentation.o\
	 neural_network.o

ocr : ${OBJ}
	gcc ${OBJ} ${CFLAGS} -o ocr

clean :
	rm -rf *.o
	rm -rf *.out
	rm -rf ocr
