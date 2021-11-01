# OpenGL_Homeworks

This repo is a collection of all my OpenGL homeworks for the Advanced Computer Graphics subject

Name: Virtopeanu Alexandru    
Group: 1231EB

# First Exercise
## Introduction

### Controls
1 - change your shape into a square     
2 - change your shape into a hexagon    
3 - change your shape into a octagon     

T - Translate your shape left to right across the screen     
S - Scale your shape up and down     
R - Rotate your shape sinusoidally (left and right)     
N - Stop current transform         

### Pseudo-OOP design

A compromise between using classes and plainly writing iterative code, I chose to use namespaces to organize methods / variables and code functionality. The main reason of not using classes is that writing boilerplate code is no longer an issue (which saves dev time) and also that I'm less bound to waste time planning ahead (and can just write the functional parts and be done with it).

Besides, I just wanted to try this approach out.

Examples: 

`shapes::SelectedShape` of type `shapes::SHAPE_TYPE` is the variable that stores what the user would like to draw next.

`shapes::square::vertices` stores the vertex data for drawing a square.

`transforms::bounds::setNextTransforms` handles switching between transform types, depending on desired user functionality



### Drawing styles

For drawing the octagon we can use only 6 triangles (instead of 8), by drawing a square and then a triangle pointing out of each of it's sides (instead of using the center point to branch out 8 triangles). 

The shape is initially scaled to 20% of its size
