# Starship Fontana #

This is a fork of [AidanDelaney/StarshipFontana][StarshipFontana] project.
Modifications made to this code are intended for 
submission as part of a university assignment.

## Story ##
_The evil b’Kuhn has stolen the code to Earth’s defence system.
With this code he can, at any time, defeat the entire human race.
Only one woman is brave enough to go after b’Kuhn. Will she be
Earth’s hero? Puzzle your way though the universe in the company
of Commander Fontana in **Starship Fontana**._



## Installation ##
Installation has been updated to make use of our Makefile, 
for added convenience.

```bash
$ make c
$ make o
```

which will produce an executable file called "starship" in the
top-level directory.  To execute this file do the following

`$ ./starship`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
Updated sprites credited to the [KenneyDonation Game Assets](http://kenney.itch.io/kenney-donation) and are used under the terms of the [CC0](http://creativecommons.org/publicdomain/zero/1.0/ "Creative Commons Zero") license.

[StarshipFontana]: https://github.com/AidanDelaney/StarshipFontana

---

## Assignment 2015 ##

_The following submitted as assignment materials, reflection and learning outcomes._

### Game Design Spec

_Following the resounding success of the Starship Fontana Meteors DLC, the franchise expands further to incorporate wildy popular paid community mods._

The Starship Fontana arcade remaster continues the open-ended journey of the mysterious meteors. One meteor, showing artificial movement patterns, leads Captain Fontana to the source hub, where a wormhole resides connecting hostile alien intelligence from a dark shadow of the universe.

Follow the Captain on a final journey, taking a brute force or careful preparation strategy to reach the wormhole. Alleviate the universe of this threat using next generation arcade movement and graphics.

#### Features

- Up, Down, Left, Right player ship movement
- New wall obstacle to house the alien source hub
- Wormhole end-game condition, main story objective
- Hostile alien guards obstructing the wormhole
- Game win-lose conditions and screens
- Collectable item, optional objective

#### Stretch Goals

- Game start screen on load
- Start / title screen audio
- Player ship firing audio
- Comet impact audio

---

_The following details steps involved in completing my submission - commentary is also added inline with modifications to the original code._

### First Phase

_SFEvent.h Extend this with a SFEVENT_PLAYER_UP and SFEvent.cpp Extend this to translate the SDL key up event to an SFEvent. Thereafter, you may want to look at SFApp::Event to handle your new SFEVENT_PLAYER_UP. Finally, you may want to add some “go up” functionality to the SFAsset._

- extend [Comet DLC PR](https://github.com/AidanDelaney/StarshipFontana/pull/2) (from lab work)
- add up-down movement to player ship

### Second Phase

_You should note that there may be an error in my collision detection code. Therefore the collisions may be off by half the width of the sprite. For the moment we just have to live with this._

- implement a wall sprite
- place wall sprites in map
- detect wall collision
- prevent player movement on wall collision

### Third Phase

- implement enemy alien sprite
- place enemy alien sprites in map
- detect shot collision with alien and player sprites

### Fourth Phase

- implement end-of-level goal asset
- place end-of-level sprite in map
- detect player collision with end-of-level goal
- implement game win condition screen
- implement game lose condition screen

### Fifth Phase

- implement collectable asset
- place collectable sprite in map
- integrate collectable detection and score in game
- implement game start / reset screen
- implement game audio
- add start screen audio
- add player shooting audio
- add comet explosion audio

### Conclusion

- write ethics and learning outcome to accompany assignment
    - attempt at deltaTime movement physics