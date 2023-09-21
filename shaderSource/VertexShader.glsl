#version 400 core

layout (location = 0) in vec4 vertexPosition_modelspace;
layout (location = 2) in vec2 vertexUV;
layout (location = 4) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = vertexUV;
    gl_Position = projection * view * aInstanceMatrix * vertexPosition_modelspace; 
}