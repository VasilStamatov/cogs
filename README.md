# cogs

COGS is a game engine which I am currently developing.
It's build with the entity-component system style,
and uses SDL2 for window and input and opengl core for rendering

Supported platform: Windows

To build the engine, follow these steps:
1. clone/checkout the repo somewhere
2. create a libraries folder in the dependencies folder
3. create a Debug and Release folder in the libraries folder
4. in the Debug folder add:
	- assimp-vc120-mtd.lib
	- BulletCollision_Debug.lib
	- BulletDynamics_Debig.lib
	- glew32.lib
	- LinearMath_Debug.lib
	- SDL2.lib
	- SDL2main.lib
	- soil2-debug.lib
5. in the Release folder add:
	- assimp-vc120-mtd.lib
	- BulletCollision.lib
	- BulletDynamics.lib
	- glew32.lib
	- LinearMath.lib
	- SDL2.lib
	- SDL2main.lib
	- soil2.lib
6. create a bin folder in the solution directory
7. create a Debug and Release folder in the bin folder
8. in the bin/Debug folder add:
	- assimp-vc120-mtd.dll
	- glew32.dll
	- SDL2.dll
9. in the bin/Release folder add:
	- assimp-vc120-mt.dll
	- glew32.dll
	- SDL2.dll
	
Now you should be able to build the engine and create a project connected to it and build that as well

Uses C++11/14 functions so you have to be able to compile that too.