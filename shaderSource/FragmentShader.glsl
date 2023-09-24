#version 400 core
out vec4 FragColor;

in vec2 TexCoords;
in float FboxID;

uniform sampler2D dirt;
uniform sampler2D grass_side;
uniform sampler2D grass_top;
uniform sampler2D SNOW_SNOW_BLOCK;
uniform sampler2D SNOW_STONE;

void main()
{
    int FboxID = int(FboxID);
    if (FboxID == 0)
        FragColor = texture(dirt, TexCoords);
    else if(FboxID == 1)
        FragColor = texture(grass_side, TexCoords);
    else
        FragColor = texture(grass_top, TexCoords);
}