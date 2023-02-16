# slow_motion_of_charged_particles
# OVERVIEW
This project utilizes FreeGlut in order to display electric field and potential around two charged particles. Positively charged particle is fixed in the center of the screen, while negative particle revolves around it in a predefined path, making this project animation, not the simulation. The program is written in C++ and uses N threads for calculations that can be specified in main.cpp file using the global define NumberOfThreads. Also, in the same file display resolution can be specified using global defines. A quick demo of animation running is available here: https://youtu.be/NniJddk_ZSk
# Requirements:
This application relies on FreeGlut (a free-software/open-source alternative to the OpenGL Utility Toolkit (GLUT) library) for rendering purpose. FreeGlut project is available on GitHub at: https://github.com/FreeGLUTProject/freeglut and can be built easily by following installation guide (read README.cmake for cmake build and README.win32 for installation on windows).
# Compilation and usage:
This project comes with visual studio solution (slow_motion_of_charged_particles.sln) file that can be opened with Visual Studio and built if all requirements are satisfied. Alternatively, use g++ on Linux to compile main.cpp and particle.cpp in slow_motion_of_charged_particles directory.
To enter the full screen press ‘f’ key, to exit press any other key.
Debug output can be enabled by uncommenting //#define DebugConsole in particle.hpp file at the expense of a few frames per second.