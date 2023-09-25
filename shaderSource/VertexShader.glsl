#version 400 core

layout (location = 0) in vec4 vertexPosition_modelspace;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vertexPosition_modelspace; 
}