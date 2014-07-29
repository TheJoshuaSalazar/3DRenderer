#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 surfaceNormal;
layout (location = 3) in vec2 UV;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 modelToWorldNormalMatrix;
uniform vec4 shapeColor;

uniform mat4 depthBiasMVP;

out vec4 Color;
out vec3 Normal;
out vec2 TexCoord; //UV
out vec4 Position; //WorldSpace
out mat3 TBN;

out vec4 ShadowCoord;

void main()
{
	Position = model * vec4(position,1);
	gl_Position = mvp * vec4(position,1);

	TexCoord = UV;
	Color = shapeColor;
	Normal = normalize(modelToWorldNormalMatrix * surfaceNormal); 

	ShadowCoord = depthBiasMVP * vec4(position, 1);

	//vec3 VertexTangent_cameraSpace = modelToWorldNormalMatrix * tangent;
	//vec3 VertexBitangent_cameraSpace = modelToWorldNormalMatrix * bitangent;
	//vec3 VertexNormal_cameraSpace = modelToWorldNormalMatrix * surfaceNormal;

	vec3 getBitangent = cross(surfaceNormal, tangent);

	TBN = mat3(tangent, getBitangent, surfaceNormal);
	TBN = modelToWorldNormalMatrix * TBN;
}