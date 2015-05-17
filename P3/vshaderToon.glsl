#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


IN vec4 vPosition;
IN vec3 vNormal;
IN vec2 vCoordTexture;

OUT vec3 Normal;
OUT vec2 v_texcoord;


uniform mat4 model_view;
uniform mat4 projection;

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
uniform tipusLlum light2;
uniform tipusLlum light3;
uniform vec4 LuzAmbiente;


uniform bool conTextura;


void main()
{
  gl_Position = projection * model_view * vPosition;
  //gl_Position = gl_Position/gl_Position.w;
  gl_Position[3] = 1.0;

  Normal = normalize(vNormal);
  v_texcoord = vCoordTexture;

}
