#version 400

uniform sampler2D NoiseWood;
uniform vec4 DarkWoodColor = vec4(0.8, 0.5, 0.1, 1.0);		
uniform vec4 LightWoodColor = vec4(1.0, 0.75, 0.25, 1.0);
uniform mat4 Slice;	
uniform float LowThreshold;
uniform float HighThreshold;

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 HalfVector;
uniform float Shininess;
uniform float Strength;
uniform sampler2D shadowMap;
uniform float alpha;

in vec4 ShadowCoord;
in vec4 Color;

out vec4 FragColor;

vec3 phongModel()
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

	return rgb;
}

void main()
{
	vec2 tc = TexCoord;
	//if(tc.s > 0.5)
	//	tc.s = 1 - tc.s;

	vec4 cyl = Slice * vec4(tc, 0.0, 1.0);
	float dist = length(cyl.xz);

	vec4 noise = texture(NoiseWood, TexCoord);
	dist += noise.b;
	
	float t = 1.0 - abs(fract(dist) * 2.0 - 1.0);
	t = smoothstep(0.2, 0.5, t);

	if(noise.a < LowThreshold)
		discard;
	if(noise.a > HighThreshold)
		discard;

	vec4 color = mix(DarkWoodColor, LightWoodColor, t);
	FragColor = vec4(phongModel() * color.rgb, 1);
}