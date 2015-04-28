# compile: make c
c:
	g++ -c -std=c++11 src/*.cpp

# link: make o
o:
	g++ -o starship *.o -lSDL2 -lSDL2_image
