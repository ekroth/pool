#version 130

uniform mat4 matrix;

in vec2 in_Vertex;
in vec2 in_Texcoord;

out vec2 pass_Texcoord;
out vec2 pass_Position;

void main()
{
	pass_Texcoord = in_Texcoord;
	pass_Position = in_Vertex;
	gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
}