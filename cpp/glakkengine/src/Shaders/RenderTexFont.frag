#version 130
#define Effect_RGBA 0
#define Effect_RGB 1
#define Effect_R 2
#define Effect_A 3

uniform sampler2D tex;

in vec2 pass_Texcoord;
in vec4 pass_Color;
in float pass_Effect;

out vec4 out_Color;

void main()
{
	vec4 texcolor = texture(tex, pass_Texcoord);

	switch (int(pass_Effect))
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