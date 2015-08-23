uniform sampler2D tex;

void main()
{
    vec4 colors[4];

    colors[0] = texture2D(tex, gl_TexCoord[0].st);
    colors[1] = texture2D(tex, gl_TexCoord[1].st);
    colors[2] = texture2D(tex, gl_TexCoord[2].st);
    colors[3] = texture2D(tex, gl_TexCoord[3].st);

    gl_FragColor = (colors[0] + colors[1] + colors[2] + colors[3]) / 4;
}
