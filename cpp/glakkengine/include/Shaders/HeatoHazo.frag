#version 130
#include "content/Shaders/Renderers/TextureBatch/TextureFragVars.glsl"

uniform vec2 heatPosition = vec2(0,0);
uniform float heatDistance = 0.0f;
uniform float totalElapsed = 0.0f;

in vec2 pass_Position;

void main()
{
	// Calculate length from heat position
	const float theDistance = distance(pass_Position, heatPosition);
// 	const float theDistance = length(vec2(abs(pass_Position.x - heatPosition.x), max(0.0f, pass_Position.y - heatPosition.y)));

	float amplitude = (1.0f - smoothstep(1.0f, 100.0f, theDistance)) * 0.02f;

	const float wavelength = 0.5f;
	const vec2 theTexcoord = vec2(pass_Texcoord.x + amplitude * sin(totalElapsed * 8 + pass_Position.y / 32), pass_Texcoord.y);

	vec4 texcolor = texture(tex, theTexcoord);

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
		out_Color = texture(tex, theTexcoord) * pass_Color; // RGBA
		break;
	};
}