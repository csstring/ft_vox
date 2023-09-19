#version 400 core

// input
in vec3 passColor;
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec3 color;
uniform sampler2D myTextureSampler;
uniform vec3 LightPosition_worldspace;
uniform float blendingRatio;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

void main(){

	float LightPower = 50.0f;
	vec3  LightColor = vec3(1,1,1);
	vec3 Materialcolor = mix(passColor, texture( myTextureSampler, UV ).rgb, blendingRatio);

	float distance = length( LightPosition_worldspace - Position_worldspace );

	vec3 n = normalize( Normal_cameraspace );
	vec3 l = normalize( LightDirection_cameraspace );
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	vec3 E = normalize(EyeDirection_cameraspace);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	
	vec3 DiffuseColor = LightColor * Kd * LightPower * cosTheta / (distance*distance);
	vec3 AmbientColor = LightColor * Ka * LightPower;
	vec3 SpecularColor = LightColor * Ks * LightPower * pow(cosAlpha,Ns) / (distance*distance);
	color = (DiffuseColor + AmbientColor + SpecularColor) * Materialcolor;

}