#version 130

uniform sampler2D tex;
uniform vec2 heatPosition;
uniform float heatDistance;
uniform float totalElapsed;

in vec2 pass_Texcoord;
in vec2 pass_Position;

out vec4 out_Color;

void main()
{
	// Calculate length from heat position
	const float theDistance = distance(pass_Position, heatPosition);
// 	const float theDistance = length(vec2(abs(pass_Position.x - heatPosition.x), max(0.0f, pass_Position.y - heatPosition.y)));

	float amplitude = (1.0f - smoothstep(25.0f, 600.0f, theDistance)) * 0.02f;

	const float wavelength = 0.5f;
	const vec2 theTexcoord = vec2(pass_Texcoord.x + amplitude * sin(totalElapsed * 8 + pass_Position.y / 24), pass_Texcoord.y);

	out_Color = texture(tex, theTexcoord);
}