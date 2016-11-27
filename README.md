# Platformer project

This project was coded in C++ with Qt Linux. It uses OpenGL, GLEW, GLUT and [LodePNG](https://github.com/lvandeve/lodepng).

![Platformer](https://github.com/antoinenry/Platformer/blob/master/Platformer/Screenshots/4.png)

Main features
-------------
The game is a 2D platform/shoot'em up. The main idea was to combine tose two game types by allowing the player to use fire weapons as propulsion devices.
The exagerated recoil can be used to jump higher, run faster or hover in the air.
The main features I coded are:

- 2D physic engine
- Sprite animation
- Player-stalking ennemies
- Arcade type: ennemies spawn until you die
- Two weapons: shotgun and SMG
- Score counting
- Spritesheet-based function to write menus and display score
- Fast-paced movements: wall jump and recoil boost

How to play
-----------
The executable should run on Ubuntu with OpenGL installed.
**Controls:** 	Q,D to move right and left
		Z to jump
		S to duck (animation missing...)
		A, E to select SMG and shotgun, or draw/undraw weapon
		Mouse to aim
		Left mouse button to fire
		Right mouse button to reload
You can grab on to ledges when your hands are free...

![Platformer](https://github.com/antoinenry/Platformer/blob/master/Platformer/Screenshots/1.png)
