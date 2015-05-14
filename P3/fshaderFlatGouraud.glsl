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
uniform bool conTextura;

void main()
{
    if(conTextura == true){
        gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
    }else{
        gl_FragColor = color;
    }
}

