#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec2 v_texcoord;

IN vec3 pos;
IN vec3 L1;
IN vec3 L2;
IN vec3 L3;
IN vec3 N;
IN float distance1;
IN float distance2;
IN float distance3;

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


uniform sampler2D texMap;
uniform bool conTextura;

void main()
{
  
  vec3 E = normalize( -pos );//vector de la pos al eye. En coordenadas de camara el eye esta en el origen
  vec3 H1 = normalize( L1 + E );
  vec3 H2 = normalize( L2 + E );
  vec3 H3 = normalize( L3 + E );
  
  

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

  vec4 color =  LuzAmbiente * material.cA  + Ilum1 + Ilum2 + Ilum3;
  color[3] = 1.0;

   if(conTextura == true){
       gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
   }else{
       gl_FragColor = color;
   }
}

