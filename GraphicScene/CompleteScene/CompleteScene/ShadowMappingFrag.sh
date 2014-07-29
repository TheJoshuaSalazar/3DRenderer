#version 400 core

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 HalfVector;
uniform float Shininess;
uniform float Strength;

uniform sampler2D shadowMap;

in vec4 ShadowCoord;
in vec4 Color;
in vec3 Normal;

out vec4 FragColor;

void main()
{
	float cosTheta = clamp( dot( Normal, normalize(LightDirection) ), 0,1 );
	float bias = 0.005 * tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.01);
	float visibility = 1.0;

	if(texture(shadowMap, ShadowCoord.xy).z < ShadowCoord.z - bias)
	{
		visibility = 0.5;
	}

	float diffuse = max(0.0, dot(Normal, LightDirection));
	float specular = max(0.0, dot(Normal, HalfVector));

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess);

	vec3 scatteredLight = Ambient + visibility * LightColor * diffuse;
	vec3 reflectedLight = visibility * LightColor * specular * Strength;

	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	FragColor = vec4(rgb, Color.a);
}