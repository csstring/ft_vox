#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D SNOW_EMERALD_ORE;
uniform sampler2D SNOW_ICE;
uniform sampler2D SNOW_POWDER_SNOW;
uniform sampler2D SNOW_SNOW_BLOCK;
uniform sampler2D SNOW_STONE;

void main()
{
    FragColor = texture(SNOW_ICE, TexCoords);
}