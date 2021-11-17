# OpenGL_Homeworks

This repo is a collection of all my OpenGL homeworks for the Advanced Computer Graphics course

Name: Virtopeanu Alexandru    
Group: 1231EB

# First Exercise

### Controls
1 - change your shape into a square     
2 - change your shape into a hexagon    
3 - change your shape into a octagon     

T - Translate your shape left to right across the screen     
S - Scale your shape up and down     
R - Rotate your shape sinusoidally (left and right)     
N - Stop current transform         

### Pseudo-OOP design

A compromise between using classes and plainly writing iterative code, I chose to use namespaces to organize methods / variables and code functionality. **why?**: way faster dev time than OOP (no boilerplate code, code just "flows" into place and less need of thinking ahead).

Besides, I just wanted to try this approach out.

Examples: 

`shapes::SelectedShape` of type `shapes::SHAPE_TYPE` is the variable that stores what the user would like to draw next.

`shapes::square::vertices` stores the vertex data for drawing a square.

`transforms::bounds::setNextTransforms` handles switching between transform types, depending on desired user functionality



### Drawing styles

For drawing the octagon we can use only 6 triangles (instead of 8), by drawing a square and then a triangle pointing out of each of it's sides (instead of using the center point to branch out 8 triangles). 

The shape is initially scaled to 20% of its size

### Bonus features

1. Crude bounds checking when scaling/translating, which reverses the direction of the transform
2. The shape changes its color constantly.
3. Sinusoidal "rotation" (back and forth)

### Quirks

Some error with bounds checking in the corners makes scaling become "stuck". Hard to reproduce by a regular user

--------

# Bowling

### Controls

W, A, S, D - translation of camera      
Mouse X/Y axis - rotation of camera      
Left Click - play animation       
Right Click - reset animation   

### Missing features

Homework not up to spec: Missing directional launch of bowling ball (launched towards pins indiferrently of where camera is pointing) and there's no bowling alley (plane) drawn

### Bowling pin drawing

To draw bowling pins we use the "Popic" (:D) class, this class creates 10 pins.

**Why use a class?**:     
- all pins are the same model,    
- the specs of the exercise just asks to animate all of the pins in the same way, so it's easier to refer to them as a whole
- we can get the pos of each bowling pin dinamically: 4 rows, each row contains (row num) bowling pins. 

### Bonus features

- mouse camera controls, with clamping at roughly 90 degrees up/down
