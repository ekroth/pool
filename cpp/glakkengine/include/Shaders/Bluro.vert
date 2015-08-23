/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#version 130
#include "content/Shaders/Renderers/TextureBatch/TextureVertVars.glsl"

uniform vec2 radius = vec2(0.006, 0.006);

out vec2 pass_Texcoords[4];

void main()
{
	pass_Color = in_Color;
	pass_Texcoord = in_Texcoord;
	pass_Effect = in_Effect;

	pass_Texcoords[0] = in_Texcoord + vec2(radius.x, radius.y);
	pass_Texcoords[1] = in_Texcoord + vec2(-radius.x, radius.y);
	pass_Texcoords[2] = in_Texcoord + vec2(-radius.x, -radius.y);
	pass_Texcoords[3] = in_Texcoord + vec2(radius.x, -radius.y);

	gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
}