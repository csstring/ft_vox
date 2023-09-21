#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D myTextureSampler;

void main()
{
    FragColor = texture(myTextureSampler, TexCoords);
}