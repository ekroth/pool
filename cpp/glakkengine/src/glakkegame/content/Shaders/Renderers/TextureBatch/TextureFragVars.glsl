/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#define Effect_RGBA 0 	// Using RGBA in texture.
#define Effect_RGB 1	// Using RGB in texture.
#define Effect_R 2		// Using R in texture as alpha.
#define Effect_A 3		// Using A in texture as alpha.

uniform sampler2D tex;

in vec2 pass_Texcoord;
in vec4 pass_Color;
in float pass_Effect;

out vec4 out_Color;