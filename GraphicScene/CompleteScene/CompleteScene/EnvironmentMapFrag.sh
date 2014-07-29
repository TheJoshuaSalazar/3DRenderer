#version 400

in vec3 Normals;
in vec3 Position;

out vec4 FragColor;

uniform samplerCube tex;

void main()
{
	vec3 tc = reflect(Position, Normals);
	FragColor = vec4(0.4, 0.4, 0.4, 1.0) + 
				vec4(0.97, 0.83, 0.79, 0.0) * 
				texture(tex, vec3(-tc.x, tc.y, -tc.z));
}