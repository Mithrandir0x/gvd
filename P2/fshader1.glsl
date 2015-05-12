#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 color;
IN vec2 v_texcoord;

uniform sampler2D texMap;

void main()
{
   gl_FragColor = color;
   //gl_FragColor = texture2D(texMap, v_texcoord);
   //gl_FragColor = color * texture2D(texMap, v_texcoord);
}

