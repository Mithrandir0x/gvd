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

OUT vec4 color;
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

struct tipusMaterial{
    vec4 cA;
    vec4 cD;
    vec4 cS;
    float coefRefEsp;
};

uniform tipusMaterial material;


void main()
{
  gl_Position = projection * model_view * vPosition;
  gl_Position = gl_Position/gl_Position.w;

  vec3 pos = (model_view * vPosition).xyz;
  vec3 L1 = normalize( light1.gpuLightPosition.xyz - pos );
  vec3 E = normalize( -pos );//vector de la pos al obs. en coordenadas de camara el obs esta en el origen
  vec3 H1 = normalize( L1 + E );
  vec3 N = normalize( model_view*vec4(vNormal, 0) ).xyz;
  float distance = length(- pos); //en coordenadas de camara el obs esta en el origen

  vec4 ambient1 = light1.gpuIa * material.cA;
  vec4 diffuse1 = light1.gpuId * material.cD * max( dot(L1, N), 0.0 );
  vec4 specular1 = max(pow(max(dot(N, H1), 0.0), material.coefRefEsp) * light1.gpuIs * material.cS , 0.0);
  float attDist1 = 1.0/(light1.gpuAtt_constant + light1.gpuAtt_linear * distance + light1.gpuAtt_quadratic * pow(distance,2.0) );

  //color = vec4(light1.gpuId.x, light1.gpuId.y, light1.gpuId.z, 1);// para debugging
  color = LuzAmbiente + attDist1 * (ambient1 + diffuse1 + specular1);
  v_texcoord = vCoordTexture;

}
