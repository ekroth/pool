#version 130
#include "content/Shaders/Renderers/TextureBatch/TextureVertVars.glsl"

out vec2 pass_Position;

void main()
{
	pass_Color = in_Color;
	pass_Texcoord = in_Texcoord;
	pass_Effect = in_Effect;
	pass_Position = in_Vertex;
	gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
}