# PA10: Pinball

# This repository contains the code for the entire project team's PA10

# Team Members
Alex Tait.
Andrew Penrose.
Austin Yount.

# Functionality
This project implements builds on PA9 and includes a "complete" pinball machine.
Spotlight is only partly working for the Phong lighting only. It doesn't point straight at the ball.

## Controls
The following controls are used while running the project:

Down Arrow: Launch Ball
Left Arrow: Left Paddle
Right Arrow: Right Paddle
N: New Game
B: Change Ambient Light Value
I: Zoom In
K: Zoom Out
J: Orbit Counterclockwise
L: Orbit Clockwise 
U: Move Camera Up
O: Move Camera Down
A: Select an object so that individual object lighting parameters can be adjusted
S: Cycles the currently selected objects specular brightness.
D: Cycles the currently selected object's diffuse brightness.
F: Cycles the currently selected object's shininess value.

# Dependencies, Building, and Running
This project requires Assimp, ImageMagick, and Bullet Physics. Other than that, There are no additional dependencies other than the standard stuff.

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev libbullet-dev
```

## Building and Running
CMake is used to build this project.

THE COMMAND TO RUN IS "./Pinball"

### CMake Instructions
The building of the project is done using CMake.

```bash
mkdir build
cd build
cmake ..
make
./Pinball
```
