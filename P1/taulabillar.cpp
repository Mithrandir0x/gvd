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

    scale = 2.0;

    readObj(n);

    Objecte::make();
    capsa = calculCapsa3D();
}

TaulaBillar::~TaulaBillar()
{

}

