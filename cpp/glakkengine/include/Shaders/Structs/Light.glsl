#define LIGHT_AMBIENT 0
#define LIGHT_SPECULAR 1
#define LIGHT_DIFFUSE 2

struct Light
{
	vec2 Position;
	vec2 Direction;
	float Type;
	float Intensity;
};