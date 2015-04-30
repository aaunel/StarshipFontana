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

"Vox Round" font used in titles licensed from TypeKit through Adobe Creative Cloud, used under [TypeKit Services Agreement](http://www.adobe.com/products/eulas/tou_typekit/ "Terms of Use").

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
- Collectible item, optional (?) objective

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

- implement collectible asset
- place collectible sprite in map
- integrate collectible detection and score in game
- implement game start / reset screen
- implement game audio
- add start screen audio
- add player shooting audio
- add comet explosion audio

### Conclusion

Writing (or rather modifying) a game in C++ has been a hugely educational experience. With a professional background in JavaScript, the C++ syntax can feel very unforgiving, but it's unquestionable that learning programming concepts from a lower-higher-level language like C++ to then apply to a higher-level language like JavaScript can make me a stronger programmer. Challenges like these will become a frequent form of practice for me.

#### LO1 Develop simple applications with relevance to the games industry

In respect to implementation of the game changes for this assignment, I'm fairly pleased with the result. The updated assets, and added game mechanics, make the game feel much more complete - can confirm the improvement since my girlfriend play-tested. I made an attempt to improve player movement in the game, to make it feel more fluid / responsive, but a hit a blocker by not being able to calculate a momentum due to no clear way to pass in a delta-time. My frustrations may be evident in the inline comments, however, I did manage to apply an acceleration multiplier to movement of objects, which gave the game some more dynamics. There's a long way to go, but I feel the basic concepts here scale well to larger games, also.

#### LO2 Use an object oriented approach to application design and development

The structure of the codebase for this game applies well to basically how any game is driven, some create-stage execution (i.e. scaffolding) and a game loop (i.e. renderer). Specifically OOP in C++ is an effective way to achieve separation of concern, we have classes for assets, collision box, events all inherited by an application class encapsulated to a few steps of loading in the `Main.cpp`. On an extension to this, I've also dabbled with some success in functional programming in JavaScript, which appeals to me for use-cases like game loops. (on a side note, C++ quine vs JS quine)

One terrible implementation in my code, that I didn't appear to be able to shake, was loading static image assets the same way as in-game (image) assets. Given a more complete understanding of an `SDL2_image` loading class, I would have separated these types of assets out, or even wrapped two subclasses for interactive and static rendered images. Hopefully, I'll look back at this portion of code with nostalgia some day.

#### LO3 Make effective use of low level features in C++

As a summation of my grasp of the C++ programming language being demonstrated in this assignment, I feel the code I've written presents a comfortable level of progress in my literacy. I didn't manage to add audio loading or extend test coverage due to knowledge gaps, to the extent that online documentation of `SDL2` could also add confusion; the ability to extend parts like these faster comes with experience of the language and libraries. My code changes are sometimes over-engineered, intended more to demonstrate variety in the assignment submissions over habit, but afaict contain only correct implementation of the language syntax - `int`, `bool`, `void`; pointers versus lists; `enum` data in `switch` statements - as well as correctly extending parts of the existing source to achieve my final result. Completed my entire plan, also; there was definitely a part of me that feared I wouldn't venture far from my meteor dlc pull request!
