# Description
This is a program implemented for practical part of the course Computer Graphics at Technische Hochschule Ulm

It consists of a 3D model created using Blender 2.8, imported to a C++ program and rendered using OpenGL 4+. It's a simulation of a
3D first person shooter with some basic shading and a minimap included.

The code includes Phong's shading model, a flashlight implementation in the fragment shader, and mouse movement/key presses for calculating the transformation matrixes applied to the 3D scene. It's far from perfect or realistic, but maybe helps other people looking for examples of implementation.

This is a fork from prof. @Alfred-Franz that contained the red triangle program (the hello world of computer graphics, so I heard) that I used as a template for creating an OpenGL project.

# Goal
The practical part of the course consisted in creating 3 showcases:

* 2D showcase containing some 2D transformations (minimap)
* Shaders showcase (Light from the moon and flashlight)
* 3D showcase containing 3D transformations to move the camera around the map.

The resulting program is a combination of the three

## Building
### Requirements
* Microsoft Visual Studio (2019 used in this case) with C++ compiler
* CMake 3+

Using CMake, select this repo's folder as source and a folder for binaries, configure, generate and open the project.

Once in MSV, find the project "FPSGame" and build. 

Execute the .exe in /FPSGame/

## Pictures
![Example of light of the moon in the scene](https://i.ibb.co/qJRRjTS/final-result2.png)
![Example of the flashlight's beam on a wall](https://i.ibb.co/5KJgVQb/Final-result.png)

## Resources
* https://learnopengl.com/
* http://www.opengl-tutorial.org/
* https://stackoverflow.com/
* https://www.khronos.org/registry/OpenGL-Refpages/gl4/
* http://www.shaderific.com/glsl-functions
