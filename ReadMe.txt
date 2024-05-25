# Project by Max Sherwin(Fanshawe Student : 2023-2024 | Id : 1128059)

##Description :
	A Gems_Project_02 simulation : The game scene displays a huge maze preloaded for the player to move around in a free flow camera and when clicking on the mouse can spawn a number of spheres that collide with the environment. I've used a bunch of threads to make the simulation smoother(explained in the video).

###Build : 
	- The solution "Gems_Project_02.sln" needs to be built(either in Debug or Release). It has two projects within it. One Static Library and the other actual project that uses the library.
	- Make the project "Game_Scene" as startup project. 
	- Build "MaxGameEngine" project first and then build "Game_Scene" project.

####Controls :
	- Free-Flow Cam Controls:
		- [W,A,S,D,Q,E] for cam movement(Front, Left, Back, Right, Up, Down respectively)
		- [Mouse] to rotate camera
		- [Spacebar] to pause camera rotation(By default it is paused)
		- [Mouse Left-Click] to spawn balls(in the direction the camera is facing). Maximum of 30 balls can be spawned.
	- [I] Key to make AABBs visible in the simulation. 
