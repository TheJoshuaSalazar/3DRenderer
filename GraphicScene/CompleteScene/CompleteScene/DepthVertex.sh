#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 surfaceNormal;

uniform mat4 depthBiasMVP;
uniform mat3 modelToWorldNormalMatrix;

void main()
{
	gl_Position = depthBiasMVP * vec4(position, 1);
}