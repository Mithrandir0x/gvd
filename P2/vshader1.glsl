#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


IN vec4 vPosition;

IN vec4 vColor;
IN vec2 vCoordTexture;

uniform mat4 model_view;
uniform mat4 projection;

OUT vec4 color;
OUT vec2 v_texcoord;


void main()
{
  gl_Position = projection * model_view * ( vPosition / vPosition.w );

  color = vColor;
  
  // Pas de les coordenades de textura al fragment shader
  // El valor del color i les coordenades de textura s'interpolaran automaticament
  // en els fragments interiors a les cares dels polígons
  v_texcoord = vCoordTexture;
}
