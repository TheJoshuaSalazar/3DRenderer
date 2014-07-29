#version 400

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 HalfVector;
uniform float Shininess;
uniform float Strength;

in vec4 Position;
in vec3 Normal;

out vec4 FragColor;

void main()
{
	vec4 Color = vec4(0.8, 0, 0, 1);
	float diffuse = max(0.0, dot(Normal, LightDirection));
	float specular = max(0.0, dot(Normal, HalfVector));

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess);

	vec3 scatteredLight = Ambient * LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;

	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	FragColor = vec4(rgb, Color.a);
}