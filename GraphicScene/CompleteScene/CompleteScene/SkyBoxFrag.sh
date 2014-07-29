#version 400

in vec3 tex_coord;

out vec4 FragColor;

uniform samplerCube tex;

void main()
{
	FragColor = texture(tex, vec3(tex_coord.x, -tex_coord.y, tex_coord.z));
}