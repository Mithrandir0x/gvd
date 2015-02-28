#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 color_from_vertexshader;

void main()
{
    gl_FragColor = color_from_vertexshader;
}

