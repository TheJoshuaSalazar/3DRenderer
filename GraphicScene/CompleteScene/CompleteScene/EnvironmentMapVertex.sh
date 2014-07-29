#version 400

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 surfaceNormal;

out vec3 Normals;
out vec3 Position;

uniform mat4 mvp;
uniform mat3 modelToWorldNormalMatrix;
uniform mat4 ViewModel;

void main()
{
	gl_Position = mvp * vec4(position, 1);

	Normals = vec3(ViewModel * vec4(surfaceNormal, 0));
	Position = vec3(ViewModel * vec4(position,1));
}