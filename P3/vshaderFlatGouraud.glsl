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
uniform bool conTextura;

// 3 luces
void main()
{
  gl_Position = projection * model_view * vPosition;
  //gl_Position = gl_Position/gl_Position.w;
  gl_Position[3] = 1.0;

  vec3 pos = (model_view * vPosition).xyz;
  vec3 L1 = normalize( (model_view * light1.gpuLightPosition).xyz - pos );
  vec3 L2 = normalize( (model_view * light2.gpuLightPosition).xyz - pos );
  vec3 L3 = normalize( (model_view * light3.gpuLightPosition).xyz - pos );
  vec3 E = normalize( -pos );//vector de la pos al eye. en coordenadas de camara el eye esta en el origen
  vec3 H1 = normalize( L1 + E );
  vec3 H2 = normalize( L2 + E );
  vec3 H3 = normalize( L3 + E );
  vec3 N = normalize( model_view*vec4(vNormal, 0.0) ).xyz;
  float distance1 = length(light1.gpuLightPosition - vPosition);
  float distance2 = length(light2.gpuLightPosition - vPosition);
  float distance3 = length(light3.gpuLightPosition - vPosition);

  vec4 ambient1 = light1.gpuIa * material.cA;
  vec4 diffuse1 = light1.gpuId * material.cD * max( dot(L1, N), 0.0 );
  vec4 specular1 = max(pow(max(dot(N, H1), 0.0), material.coefRefEsp) * light1.gpuIs * material.cS , 0.0);
  float attDist1 = 1.0/(light1.gpuAtt_constant + light1.gpuAtt_linear * distance1 + light1.gpuAtt_quadratic * pow(distance1,2.0) );
  vec4 Ilum1 = attDist1 * (ambient1 + diffuse1 + specular1);

  vec4 ambient2 = light2.gpuIa * material.cA;
  vec4 diffuse2 = light2.gpuId * material.cD * max( dot(L2, N), 0.0 );
  vec4 specular2 = max(pow(max(dot(N, H2), 0.0), material.coefRefEsp) * light2.gpuIs * material.cS , 0.0);
  float attDist2 = 1.0/(light2.gpuAtt_constant + light2.gpuAtt_linear * distance2 + light2.gpuAtt_quadratic * pow(distance2,2.0) );
  vec4 Ilum2 = attDist2 * (ambient2 + diffuse2 + specular2);

  vec4 ambient3 = light3.gpuIa * material.cA;
  vec4 diffuse3 = light3.gpuId * material.cD * max( dot(L3, N), 0.0 );
  vec4 specular3 = max(pow(max(dot(N, H3), 0.0), material.coefRefEsp) * light3.gpuIs * material.cS , 0.0);
  float attDist3 = 1.0/(light3.gpuAtt_constant + light3.gpuAtt_linear * distance3 + light3.gpuAtt_quadratic * pow(distance3,2.0) );
  vec4 Ilum3 = attDist3 * (ambient3 + diffuse3 + specular3);

  color =  LuzAmbiente * material.cA  + Ilum1 + Ilum2 + Ilum3;
  color[3] = 1.0;
  v_texcoord = vCoordTexture;

}
