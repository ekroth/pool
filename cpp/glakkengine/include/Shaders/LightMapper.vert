#version 130
#include "content/Shaders/Structs/Light.glsl"
// Don't translate vertices with matrix
#define MODE_STATIC 0
// Translate vertices with matrix
#define MODE_CAMERA 1

uniform Light light;
uniform mat4 matrix;
uniform float mode = MODE_STATIC;

in vec2 in_Vertex;

out vec2 pass_Position;
out vec2 pass_LightPosition;

void main()
{
	switch (int(mode + 0.5f))
	{
		case MODE_STATIC:
			pass_Position = in_Vertex;
			//const vec4 lightPos = matrix * vec4(light.Position, 0.0, 1.0);
			//pass_LightPosition = lightPos.xy;
			pass_LightPosition = light.Position;

			gl_Position = matrix * vec4(in_Vertex, 0.0, 1.0);
			break;
	
		case MODE_CAMERA:
			const vec4 vert = matrix * vec4(in_Vertex, 0.0, 1.0);
			pass_Position = vert.xy;
			pass_LightPosition = light.Position;
		
			gl_Position = vert;
			break;
	};
}