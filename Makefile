all: archman clean

archman: main.cpp archman.so
	g++ -Wall -Wextra main.cpp archman.so -o archman

archman.so: archman.cpp archman.h
	g++ -Wall -Wextra -c archman.cpp -o archman.so

clean:
	rm -f *.so