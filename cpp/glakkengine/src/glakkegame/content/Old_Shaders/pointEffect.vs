#version 110

attribute float position;
attribute float radie;

uniform float maxRadie;
uniform vec2 origin;

void main()
{
    vec4 pos = vec4(origin.x + sin(position) * radie, origin.y + cos(position) * radie, 0.0, 1.0);

    float val1 = tan(position);
    float val2 = sin(position);
    float val3 = cos(position);

//    float val1 = 0.5;
//    float val2 = 0.5;
//    float val3 = 1.0;

    gl_FrontColor = vec4(val2, val3, val1, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * pos;
}
