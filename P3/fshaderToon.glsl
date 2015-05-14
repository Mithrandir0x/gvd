#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec3 Normal;
IN vec2 v_texcoord;

struct tipusLlum{
    vec4 gpuLightPosition;
    vec4 gpuLightDirection;
    vec4 gpuIa;
    vec4 gpuId;
    vec4 gpuIs;
    float gpuBeamAngle;
    float gpuAtt_constant;
    float gpuAtt_linear;
    float gpuAtt_quadratic;
};

uniform tipusLlum light1;

uniform sampler2D texMap;
uniform bool conTextura;

void main()
{
    float intensity;
    vec4 color;

    intensity = dot(normalize(light1.gpuLightPosition.xyz), Normal);//si consideramos luz direccional que apunta al origen de coord. en el world

    if (intensity > 0.95)
        color = vec4(0.5, 1.0, 0.5, 1.0);
    else if (intensity > 0.5)
        color = vec4(0.3, 0.6, 0.3, 1.0);
    else if (intensity > 0.25)
        color = vec4(0.2, 0.4, 0.2, 1.0);
    else
        color = vec4(0.1, 0.2, 0.1, 1.0);
        
    if(conTextura == true){
        gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
    }else{
        gl_FragColor = color;
    }
}

