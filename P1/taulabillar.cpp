#include "taulabillar.h"

TaulaBillar::TaulaBillar(QString n) : Objecte(NumVerticesF)
{
    nom = n;
    Index = 0;
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    readObj(n);

    Objecte::make();
    capsa = calculCapsa3D();

    // Codi adhoc per a la taula carregada de fitxer taula.obj. Cal modificar-lo per a que sigui general

    double aristaMax = 0.0;

    if(capsa.a > capsa.p){
        aristaMax=capsa.a;
    } else{
        aristaMax=capsa.p;
    }
    if(capsa.h > aristaMax){
        aristaMax=capsa.h;
    }

    double escala = 2.0 / aristaMax;
    mat4 m = Scale(escala, escala, escala);
    aplicaTG(m);//la mayor arista de la caja contenedora de la mesa es 2.0

    capsa = calculCapsa3D();
    //std::cout<<"px: "<< capsa.pmin.x<< ", py: "<< capsa.pmin.y<<", pz: "<< capsa.pmin.z<<", a: "<<capsa.a<<", h: "<<capsa.h<<", p: "<<capsa.p<<"\n";

    m = Translate(-(capsa.pmin.x + capsa.a / 2.), -(capsa.pmin.y + capsa.h), -(capsa.pmin.z + capsa.p / 2.));
    aplicaTG(m);//deja el centro de la mesa(x,z) en el origen de coordenadas y el tope en el plano y=0, con arista maxima 2.0
    capsa = calculCapsa3D();
}

TaulaBillar::~TaulaBillar()
{

}

