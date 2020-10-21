files = $(currentdir)*.c 

all: 
	gcc -Wall -O -g -lm -Wextra -Werror -std=c99 -O1 -o main $(files)

clang:
		clang -Wall -g -lm -Wextra -Werror -std=c99 -O1 -o main $(files)

	
clean:
	rm -f main

