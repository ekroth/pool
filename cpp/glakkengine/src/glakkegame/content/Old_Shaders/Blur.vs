uniform float x_radius, y_radius;

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0 - vec4(-x_radius, 0.0, 0.0, 0.0);
    gl_TexCoord[1] = gl_MultiTexCoord0 - vec4(x_radius, 0.0, 0.0, 0.0);
    gl_TexCoord[2] = gl_MultiTexCoord0 - vec4(0.0, -y_radius, 0.0, 0.0);
    gl_TexCoord[3] = gl_MultiTexCoord0 - vec4(0.0, y_radius, 0.0, 0.0);

    gl_Position = ftransform();
}