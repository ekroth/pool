// Position of GL_POINT
attribute vec2 position;

void main()
{	
    vec4 p = vec4(position.x, position.y, 1.0, 1.0); 
    gl_Position = gl_ModelViewProjectionMatrix * p;
}