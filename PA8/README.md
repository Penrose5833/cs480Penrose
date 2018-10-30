# PA6: Object and Texture Loading with Assimp

# This repository contains the code for the entire project team's PA6

# Team Members
Alex Tait.
Andrew Penrose.
Austin Yount.

# Functionality
This project implements assimp model and texture loading from a user specified file path. 

To use the program, build and run as specified below. When prompted in the terminal, enter the complete filepath of the .obj file you would like to load. Note that the filepath is relative to the build directory.

The user can move the camera around to some extent with the W, A, S, D, and UP/Down arrows. 

# Dependencies, Building, and Running
This project obviously requires Assimp and ImageMagick. Other than that, There are no additional dependencies other than the standard stuff.

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev
```

## Building and Running
CMake is used to build this project.

### CMake Instructions
The building of the project is done using CMake.

```bash
mkdir build
cd build
cmake ..
make
./Solar
```
