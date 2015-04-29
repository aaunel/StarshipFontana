# Added make commands for utility, this makes compilation / linking
# steps quicker during development, and can be more easily communicated.
# Updated README.md to document make commands.

# compile: make c
c:
	g++ -c -std=c++11 src/*.cpp

# link: make o
o:
	g++ -o starship *.o -lSDL2 -lSDL2_image
