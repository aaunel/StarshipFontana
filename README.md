# Starship Fontana #

This is an example C++ application using the SDL library.
It tries to be as nicely C++11 as possible but do keep in
mind that SDL is written in C and, at some stage, you have
to interface with it.

## Story ##
The evil b’Kuhn has stolen the code to Earth’s defence system.
With this code he can, at any time, defeat the entire human race.
Only one woman is brave enough to go after b’Kuhn. Will she be
Earth’s hero? Puzzle your way though the universe in the company
of Commander Fontana in **Starship Fontana**.

## Installation ##
You will have to have the SDL development libraries installed on
your system.  The easiest way to compile is to use a command-line

```bash
$ g++ -c -std=c++11 src/*.cpp
$ g++ -o starship *.o -lSDL2 -lSDL2_image
```

which will produce an executable file called "starship" in the
top-level directory.  To execute this file do the following

`$ ./starship`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
Updated sprites credited to the [KenneyDonation Game Assets](http://kenney.itch.io/kenney-donation) and are used under the terms of the [CC0](http://creativecommons.org/publicdomain/zero/1.0/ "Creative Commons Zero") license.
