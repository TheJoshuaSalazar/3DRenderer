#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 surfaceNormal;

uniform mat4 depthBiasMVP;
uniform mat4 mvp;
uniform vec4 shapeColor;
uniform mat3 modelToWorldNormalMatrix;

out vec4 ShadowCoord;
out vec4 Color;
out vec3 Normal;

void main()
{
	Color = shapeColor;
	Normal = normalize(modelToWorldNormalMatrix * surfaceNormal);
	gl_Position = mvp * vec4(position, 1);
	ShadowCoord = depthBiasMVP * vec4(position, 1);
}