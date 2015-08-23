#version 130

uniform mat4 matrix;

in vec2 in_Vertex;
in vec2 in_Texcoord;

out vec2 pass_Texcoord;

void main()
{
	// Does nothing fancy
	pass_Texcoord = in_Texcoord;
	gl_Position = matrix * vec4(in_Vertex, 0, 1);
}