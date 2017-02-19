# cogs

COGS is a game engine which I am currently developing.
It's build with the entity-component system style,
and uses SDL2 for window and input and opengl core for rendering

Supported platform: Windows

To build the engine, follow these steps:
- clone/checkout the repo somewhere
- create a libraries folder in the dependencies folder
- create a Debug and Release folder in the libraries folder
- in the Debug folder add:
	- assimp-vc120-mtd.lib
	- BulletCollision_Debug.lib
	- BulletDynamics_Debig.lib
	- glew32.lib
	- LinearMath_Debug.lib
	- SDL2.lib
	- SDL2main.lib
	- soil2-debug.lib
- in the Release folder add:
	- assimp-vc120-mtd.lib
	- BulletCollision.lib
	- BulletDynamics.lib
	- glew32.lib
	- LinearMath.lib
	- SDL2.lib
	- SDL2main.lib
	- soil2.lib
- create a bin folder in the solution directory
- create a Debug and Release folder in the bin folder
- in the bin/Debug folder add:
	- assimp-vc120-mtd.dll
	- glew32.dll
	- SDL2.dll
- in the bin/Release folder add:
	- assimp-vc120-mt.dll
	- glew32.dll
	- SDL2.dll
	
Now you should be able to build the engine and create a project connected to it and build that as well

Uses C++11/14 functions so you have to be able to compile that too.
