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
OUT vec2 v_texcoord;

OUT vec3 pos;
OUT vec3 L1;
OUT vec3 L2;
OUT vec3 L3;
OUT vec3 N;
OUT float distance1;
OUT float distance2;
OUT float distance3;

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

struct tipusMaterial{
    vec4 cA;
    vec4 cD;
    vec4 cS;
    float coefRefEsp;
};

uniform tipusMaterial material;
uniform bool conTextura;


void main()
{
  gl_Position = projection * model_view * vPosition;
  //gl_Position = gl_Position/gl_Position.w;
  gl_Position[3] = 1.0;

  pos = (model_view * vPosition).xyz;
  L1 = normalize( (model_view * light1.gpuLightPosition).xyz - pos );
  L2 = normalize( (model_view * light2.gpuLightPosition).xyz - pos );
  L3 = normalize( (model_view * light3.gpuLightPosition).xyz - pos );
  N = normalize( model_view * vec4(vNormal, 0.0) ).xyz;

  distance1 = length(light1.gpuLightPosition - vPosition);
  distance2 = length(light2.gpuLightPosition - vPosition);
  distance3 = length(light3.gpuLightPosition - vPosition);

  v_texcoord = vCoordTexture;

}
