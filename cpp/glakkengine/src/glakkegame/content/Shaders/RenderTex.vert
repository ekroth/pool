/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#version 130

uniform mat4 matrix;

in vec2 in_Vertex;
in vec2 in_Texcoord;
in vec4 in_Color;
in float in_Effect;

out vec2 pass_Texcoord;
out vec4 pass_Color;
out float pass_Effect;

void main()
{
	pass_Color = in_Color;
	pass_Texcoord = in_Texcoord;
	pass_Effect = in_Effect;
	gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
}