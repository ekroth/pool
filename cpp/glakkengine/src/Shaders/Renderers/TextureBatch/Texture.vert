/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#version 130
#include "content/Shaders/Renderers/TextureBatch/TextureVertVars.glsl"

void main()
{
	pass_Color = in_Color;
	pass_Texcoord = in_Texcoord;
	pass_Effect = in_Effect;
	gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
}