# windows-graphics-test-1
A test of the use of the Windows API in graphics. Made for Visual Studio Community 2017

Draws two squares: One controllable red square, and one uncontrollable blue square (which exists solely to reveal when the red square does
not properly return the graphics handle to it's state pre-draw).

Control keys are:

 - [WASD] Accelerate the red square
 - [Left/Right arrows] Rotate the red square (turning the acceleration vector with it)
 - [Up/Down arrows] Scale all objects up and down in size
 - [Q/E] Increase/Decrease the size of the red square
 - [Tab] Reset the size of the red square
 - [|\] Reset the scaling of all objects
 - [Spacebar] Stop the red square
 - [Enter] Reset the rotation of the red square
