/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#version 130
#include "content/Shaders/Renderers/TextureBatch/TextureFragVars.glsl"

void main()
{
	vec4 texcolor = texture(tex, pass_Texcoord);

	switch (int(pass_Effect + 0.5f))
	{
		case Effect_RGB:
		out_Color = vec4(texcolor.r * pass_Color.r, texcolor.g * pass_Color.g, texcolor.b * pass_Color.b, pass_Color.a);
		break;

		case Effect_R:
		out_Color = vec4(pass_Color.r, pass_Color.g, pass_Color.b, texcolor.r * pass_Color.a);
		break;

		case Effect_A:
		out_Color = vec4(pass_Color.r, pass_Color.g, pass_Color.b, texcolor.a * pass_Color.a);
		break;

		default:
		out_Color = texture(tex, pass_Texcoord) * pass_Color; // RGBA
		break;
	};
}