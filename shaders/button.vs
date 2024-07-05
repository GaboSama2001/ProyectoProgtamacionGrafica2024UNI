#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
// layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 ourPosition;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    ourPosition = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // ourColor = aColor;
}