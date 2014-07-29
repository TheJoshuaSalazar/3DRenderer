#version 400

layout (location = 0) in vec3 position;

out vec3 tex_coord;
uniform mat4 mvp;

void main()
{
	tex_coord = position;
	gl_Position = mvp * vec4(position, 1);
}