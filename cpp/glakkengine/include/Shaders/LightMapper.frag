#version 130
#include "content/Shaders/Structs/Light.glsl"

uniform Light light;

in vec2 pass_Position;
in vec2 pass_LightPosition;

out vec4 out_Color;

void main()
{
	switch (int(light.Type))
	{
		case LIGHT_AMBIENT:
			out_Color = vec4(light.Intensity, light.Intensity, light.Intensity, 1);
			break;

		case LIGHT_SPECULAR:
			const float dist = distance(pass_LightPosition, pass_Position);
			const float intensity = (1.0f - smoothstep(0, light.Direction.x, dist)) * light.Intensity;
			out_Color = vec4(intensity, intensity, intensity, 1);
			break;

		default:
			out_Color = vec4(1, 0, 0, 1); // Debug
			break;
	};
}