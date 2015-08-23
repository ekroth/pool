/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#version 130
#include "content/Shaders/Renderers/TextureBatch/TextureFragVars.glsl"

in vec2 pass_Texcoords[4];

void main()
{
	pass_Texcoord; // Ignore
	vec4 avgcolor;
	
	for (int i = 0; i < 4; i++)
	{
		vec4 texcolor = texture(tex, pass_Texcoords[i]);
// 		vec4 texcolor = texture(tex, max(vec2(0, 0), pass_Texcoords[i]));

		switch (int(pass_Effect + 0.5f))
		{
			case Effect_RGB:
			avgcolor += vec4(texcolor.r * pass_Color.r, texcolor.g * pass_Color.g, texcolor.b * pass_Color.b, pass_Color.a);
			break;

			case Effect_R:
			avgcolor += vec4(pass_Color.r, pass_Color.g, pass_Color.b, texcolor.r * pass_Color.a);
			break;

			case Effect_A:
			avgcolor += vec4(pass_Color.r, pass_Color.g, pass_Color.b, texcolor.a * pass_Color.a);
			break;

			default:
			avgcolor += texcolor;
			break;
		};
	}

	avgcolor /= 4.0;

	out_Color = avgcolor;
}