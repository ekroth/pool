#version 130

uniform mat4 matrix;
uniform vec2 radius = vec2(0.002, 0.002);

in vec2 in_Vertex;
in vec2 in_Texcoord;

out vec2 pass_Texcoords[4];

void main()
{
	pass_Texcoords[0] = in_Texcoord + vec2(radius.x, radius.y);
	pass_Texcoords[1] = in_Texcoord + vec2(-radius.x, radius.y);
	pass_Texcoords[2] = in_Texcoord + vec2(-radius.x, -radius.y);
	pass_Texcoords[3] = in_Texcoord + vec2(radius.x, -radius.y);
	gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
}