#version 400

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 surfaceNormal;

uniform float time;
uniform float waveNumber;
uniform float velocity;
uniform float amp;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 modelToWorldNormalMatrix;
uniform mat4 depthBiasMVP;

out vec4 Position;
out vec3 Normal;
out vec4 ShadowCoord;

void main()
{
	vec4 pos = vec4(position,1);
	
	float u = waveNumber * (pos.x - velocity * time);
	pos.y = amp * sin(u);
	
	vec3 n = vec3(0);
	n.xy = normalize(vec2(-waveNumber * amp * cos(u), 1));
	
	Position = model * pos;
	Normal = normalize(modelToWorldNormalMatrix * n); 
	ShadowCoord = depthBiasMVP * pos;

	gl_Position = mvp * pos;
}