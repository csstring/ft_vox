#version 400 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec3 Incolor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal_modelspace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 LightPosition_worldspace;

out vec3 passColor;
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

void main(){

    gl_Position = projection * view * model * vertexPosition_modelspace;
    Position_worldspace = (model * vertexPosition_modelspace).xyz;

    vec3 vertexPosition_cameraspace = ( view * model * vertexPosition_modelspace).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    vec3 LightPosition_cameraspace = (view * vec4(LightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    Normal_cameraspace = ( view * model * vec4(vertexNormal_modelspace,0)).xyz;
    
    passColor = Incolor;
    UV = vertexUV;
}