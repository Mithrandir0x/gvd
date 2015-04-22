#ifndef ESCENA_H
#define ESCENA_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>


#include <Common.h>
#include <objecte.h>
#include <taulabillar.h>
#include <plabase.h>
#include <bola.h>
#include <conjuntboles.h>
#include <camera.h>
#include <math.h>

using namespace std;

class Escena
{
public:
    Escena(int vpa, int vph);
    ~Escena();

    void addObjecte(Objecte *obj);
    void aplicaTG(mat4 m);

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw();
    void CapsaMinCont3DEscena();

    void iniCamera(bool camGen, int vpa, int vph);
    void setAnglesCamera(bool camGeneral, float angX, float angY, float angZ);
    void setVRPCamera(bool camGeneral, point4 vrp);
    void setWindowCamera(bool camGeneral, bool retallat, Capsa2D window);
    void setDCamera(bool camGeneral, float d);
    void tuneCamera(bool retallat, bool centrat, QGLShaderProgram *program);

    QGLShaderProgram *pr;
    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;

    TaulaBillar *taulaBillar;
    PlaBase *plaBase;
    Bola *bolaBlanca = NULL;
    ConjuntBoles *conjuntBoles;
    Camera camGeneral;
    vector<Objecte*> listaObjectes;
};

#endif // ESCENA_H
