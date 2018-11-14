# PA9: Lighting

# This repository contains the code for the entire project team's PA9

# Team Members
Alex Tait.
Andrew Penrose.
Austin Yount.

# Functionality
This project implements builds on PA8 with the addition of lighting.
Spotlight is only partly working for the Phong lighting only. It doesn't point straight at the ball.

## Controls
The following controls are used while running the project:

T: toggles between Gourand and Phong shaders
B: cycles the ambient light value. The value loops through values between  0 and 1 in increments of 0.1.
A: select an object so that individual object lighting parameters can be adjusted.
S: cycles the currently selected objects specular brightness.
D: cycles the currently selected object's diffuse brightness.
F: cycles the currently selected object's shininess value.

Up/Down/Left/Right arrows: used to shift the gravity around so that the objects move. Note the gravity shifts on keydown and returns to center on keyup. 

# Dependencies, Building, and Running
This project requires Assimp, ImageMagick, and Bullet Physics. Other than that, There are no additional dependencies other than the standard stuff.

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev libbullet-dev
```

## Building and Running
CMake is used to build this project.
To run the project you must specify the name of the configuration file and the configuration file must be placed in the "Configuration" folder. Conveniently, this is already done for you...

THE COMMAND TO RUN IS "./Pinball objects.txt"

### CMake Instructions
The building of the project is done using CMake.

```bash
mkdir build
cd build
cmake ..
make
./Pinball objects.txt
```
