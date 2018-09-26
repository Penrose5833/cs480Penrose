# PA4: Object Loading
# Building, Running, and Using the Program


## Building and Running
To build this project there are two options. One is to use CMake which makes including new libraries easier, and handles new files added automatically to the src and include directory. CMake is a small new learning curve but makes things easier in the future.
The second option is to use the provided Makefile which is used as usual.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.  

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```
## Using

The .obj filess are placed in the objects folder inside the PA4 directory. I removed the .obj ignore to upload the board I made in blender. That is the current file path link.

```LINE 51 ```of graphics.cpp has a function that takes in the .obj file path. I overloaded this function, so no file path will produce the cubes from the previous projects.

```
The computers in dr. harris' lab are much faster, but when loading the dragon on my computer it takes about 10 seconds before anything happens.
```

To interact with the program:

Keyboard:
"s" - change rotation direction of center planet (SUN).
"a" - change rotation direction of closest planet to Sun.
"d" - change rotation direction of Farthest planet from Sun.
"f" - change orbit direction of farthest planet from the sun.
"esc" - quit
Up Arrow: change orbit direction of ALL moons.
Down arrow: change rotation direction of ALL moons.
Left arrow: change rotation direction of ALL planets and Sun.
Right arrow: change orbit direction of ALL planets.

Mouse:
Left Mouse click: change orbit direction of closest planet to the sun.





