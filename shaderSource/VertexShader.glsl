#version 400 core

layout (location = 0) in vec4 vertexPosition_modelspace;
layout (location = 1) in float boxID;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out float FboxID;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    FboxID = boxID;
    if (boxID == 1 && normal[1] == 1)
        FboxID = 2;
    TexCoords = vertexUV;
    gl_Position = projection * view * aInstanceMatrix * vertexPosition_modelspace; 
}