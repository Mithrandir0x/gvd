#include "bola.h"

Bola::Bola(double x0, double y0, double z0, double r, double R, double G, double B, QString numBola) : Objecte(NumVerticesBola){
   color.x = R;
   color.y = G;
   color.z = B;
   color.w = 1;
   make(x0,y0,z0,r,numBola);//esfera centrada en el origen de radio 1
   capsa = calculCapsa3D();
   double aristaMax = 0.0;

   if(capsa.a > capsa.p){
       aristaMax=capsa.a;
   } else{
       aristaMax=capsa.p;
   }
   if(capsa.h > aristaMax){
       aristaMax=capsa.h;
   }
   double escala = 2* r / aristaMax;

   mat4 m = Translate(x0, y0, z0)*Scale(escala, escala, escala);
   aplicaTG(m);//sale la bola con radio r y posicion x0,y0,z0
   capsa = calculCapsa3D();
}

Bola::~Bola()
{

}

void Bola::make(double x0, double  y0, double  z0, double r, QString numBola)
{
    //std::cout<<"Bola::make\n";
    // generacio de la geometria dels triangles per a visualitzar-lo

    Index = 0;

    point4 v[4] = {
        vec4(0.0, 0.0, 1.0, 1.0 ),
        vec4(0.0, 0.942809, -0.333333, 1.0 ),
        vec4(-0.816497, -0.471405, -0.333333, 1.0 ),
        vec4(0.816497, -0.471405, -0.333333, 1.0 )
    };

    divide_triangle(v[0],v[1],v[2],NumIteracionsEsfera);
    divide_triangle(v[3],v[2],v[1],NumIteracionsEsfera);
    divide_triangle(v[0],v[3],v[1],NumIteracionsEsfera);
    divide_triangle(v[0],v[2],v[3],NumIteracionsEsfera);

    initTextura(numBola);//añadido
}

void Bola::initTextura(QString numBola)
 {
     //numBola = "2";
     //qDebug() << "Initializing textures...";

     QString fileTextura = "://resources/Bola";//añadido
     fileTextura.append(numBola).append(".jpg");//añadido

     // Carregar la textura
     glActiveTexture(GL_TEXTURE0);
     texture = new QOpenGLTexture(QImage(fileTextura));  //modificado
     texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     texture->setMagnificationFilter(QOpenGLTexture::Linear);

     texture->bind(0);

 }

void Bola::divide_triangle(const vec4& a, const vec4& b, const vec4& c, int cont){

    if (cont > 0) {

        vec4 ab= normalize(a + b); //puntos interiores fijados a la esfera
        vec4 ac = normalize(a + c);
        vec4 bc = normalize(b + c);

        divide_triangle(a,ab, ac, cont-1);//comprobar orden
        divide_triangle(c,ac,bc, cont-1);
        divide_triangle(b,bc,ab, cont-1);
        divide_triangle(ab,bc,ac, cont-1);
    }
    else {
        triangle(a,b,c);
    }
}

vec4 Bola::normalize(const vec4& p)
{
    vec4 v;

    v = p / sqrt((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
    v.w = 1;

    return v;
}

void Bola::triangle(const vec4& a, const vec4& b, const vec4& c )
{
    double u, v;

    vec4 vertArr[3] = {a, b, c};

    for(int i =0; i<3;i++){
        points[Index] = vertArr[i];
        colors[Index] = this->color;
        u = 0.5 + atan2(-points[Index].z, -points[Index].x)/(2*M_PI);
        v = 0.5 - asin(-points[Index].y)/M_PI;
        if(u < 0.0)u=0.0;
        if(v < 0.0)v=0.0;
        if(u > 1.0)u=1.0;
        if(v > 1.0)v=1.0;
        vertexsTextura[Index] = vec2(u , v);
        Index++;
    }
}

