#version 130

uniform sampler2D tex;

in vec2 pass_Texcoords[4];

out vec4 out_Color;

void main()
{
	vec4 avgcolor(0);
	
	for (int i = 0; i < 4; i++)
	{
		avgcolor += texture(tex, pass_Texcoords[i]);
	}

	avgcolor /= 4.0;

	out_Color = avgcolor;
}