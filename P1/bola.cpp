#include "bola.h"

Bola::Bola(double x0, double y0, double z0, double r, double R, double G, double B) : Objecte(NumVerticesBola){
    color.x = R;
    color.y = G;
    color.z = B;
    color.w = 1;
    make(x0,y0,z0,r);
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
    double escala = 2 * r / aristaMax;
    mat4 m = Scale(escala, escala, escala)*Translate(-(capsa.pmin.x + capsa.a / 2), -(capsa.pmin.y + this->capsa.h / 2), -(capsa.pmin.z + capsa.p / 2));
    aplicaTG(m);
    capsa = calculCapsa3D();
    m = Translate(x0, y0, z0);
    aplicaTG(m);
}

Bola::~Bola()
{

}

void Bola::make(double x0, double  y0, double  z0, double r)
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

    mat4 transform = (Translate(x0,y0,z0) * Scale(r,r,r));
    for ( int i = 0; i < NumVerticesBola; ++i ) {
        points[i] = transform  * points[i];
    }

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

    points[Index] = a;
    colors[Index] = this->color;
    Index++;

    points[Index] = b;
    colors[Index] = this->color;
    Index++;

    points[Index] = c;
    colors[Index] = this->color;
    Index++;
}

