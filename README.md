Matthew Salaciak 29644490
COMP 371 Assignment 1 README files

The assignment was to create a 3D olaf, placed onto a 100 x 100 grid in the XZ plane, with XYZ coordinate lines of 5 grid length each in 3 different colours.
The assignment also required movement of the olaf, the camera and the world.

The project consists of a source.cpp file which is the main,
Vertices.h which contain all the vec3 vertices information and the color information of the XYZ vertices
The shader.h loads the shader files from the .vs and .fs files
The shaders for each object are stored in their own files.
.fs is for fragment shaders
.vs is for vertex shaders

Here are the keybindings to control the olaf, camera and world 

OLAF KEY CONTROLS:
T: Render the olaf with triangles
L: Render the olaf with lines
P: Render the olaf with points
U: Scale olaf up
J: Scale olaf  down
Space Bar: Move the olaf to a random grid location
W: Move olaf away screen
S: Move olaf towards from screen
A: Move olaf to the left
D: Move olaf to the right
N: rotate olaf on the Y axis counter clockwise
M: rotate olaf on the Y axis clockwise
I: rotate olaf on the X,Z axis clockwise
O: rotate olaf on the X,Z acid counter clockwise

CAMERA:
Mouse X left and right controls camera pan
Mouse Y controls camera tilt up and down

WORLD SCENE CONTROLS:

1: move scene to the left
2: move scene to the right
3: zoom into scene
4: zoom out of scene
5: move scene down
6: move scene up

LEFT ARROW: rotate X axis
RIGHT ARROW: rotate X axis in the other orientation
UP ARROW: rotate Y axis
DOWN ARROW: rotate Y axis in the other orientation
TAB: resets the orientation of the world to original settings.

TO EXIT PROGRAM PLEASE PRESS ESC KEY
