#version 130

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Vertex;
in vec4 in_Color;

out vec4 pass_Color;

void main()
{
	pass_Color = in_Color;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Vertex, 1.0);
}