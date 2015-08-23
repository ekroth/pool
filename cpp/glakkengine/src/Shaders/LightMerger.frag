#version 130

uniform sampler2D lightMap;
uniform sampler2D tex;

in vec2 pass_Texcoord;

out vec4 out_Color;

void main()
{
	vec3 tmp = texture(lightMap, pass_Texcoord).xyz * texture(tex, pass_Texcoord).xyz;
	out_Color.w = texture(tex, pass_Texcoord).w;
	out_Color.xyz = tmp;
}