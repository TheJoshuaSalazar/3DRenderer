#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 surfaceNormal;
layout (location = 3) in vec2 UV;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 modelToWorldNormalMatrix;

uniform mat4 depthBiasMVP;
uniform vec4 shapeColor;

out vec4 Position;
out vec3 Normal;
out vec2 TexCoord; //UV

out vec4 ShadowCoord;
out vec4 Color;

void main()
{
	TexCoord = UV;
	Color = shapeColor;
	Position = mvp * vec4(position, 1);
	Normal = normalize(modelToWorldNormalMatrix * surfaceNormal);
	ShadowCoord = depthBiasMVP * vec4(position, 1);

	gl_Position = mvp * vec4(position, 1);
}