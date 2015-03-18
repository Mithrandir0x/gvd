#include "escena.h"

Escena::Escena()
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    taulaBillar = NULL;
    plaBase = NULL;
    bolaBlanca = NULL;
}

Escena::~Escena()
{
    // Cal anar fent delete dels objectes que se'l hagi fet new
   delete this->taulaBillar;
   delete this->plaBase;
   delete this->bolaBlanca;
}

void Escena::addObjecte(Objecte *obj) {
    if (dynamic_cast<TaulaBillar*>(obj)){
        this->taulaBillar = (TaulaBillar*)obj;
    } else if (dynamic_cast<PlaBase*>(obj)){
        this->plaBase = (PlaBase*)obj;
    }else if (dynamic_cast<Bola*>(obj))
        this->bolaBlanca= (Bola*)obj;
}


void Escena::CapsaMinCont3DEscena()
{
    // Metode a implementar
}

void Escena::aplicaTG(mat4 m) {

    // Metode a modificar

    if (taulaBillar!=NULL)
        taulaBillar->aplicaTG(m);
    if (plaBase!=NULL)
        plaBase->aplicaTG(m);
    if (bolaBlanca!=NULL)
        bolaBlanca->aplicaTG(m);
    if (listaConjuntBoles.size()!=0) {
        for (int i=0; i<listaConjuntBoles.size(); i++) {
            listaConjuntBoles[i]->aplicaTG(m);
        };
    }

}

void Escena::aplicaTGCentrat(mat4 m) {

    // Metode a modificar

    if (taulaBillar!=NULL)
        taulaBillar->aplicaTGCentrat(m);
    if (plaBase!=NULL)
        plaBase->aplicaTGCentrat(m);
    if (bolaBlanca!=NULL)
        bolaBlanca->aplicaTGCentrat(m);
    if (listaConjuntBoles.size()!=0) {
        for (int i=0; i<listaConjuntBoles.size(); i++) {
            listaConjuntBoles[i]->aplicaTGCentrat(m);
        };
    }

}

void Escena::draw() {

    // Metode a modificar

    if (taulaBillar!=NULL)
        taulaBillar->draw();

    if (plaBase!=NULL)
        plaBase->draw();

    if (bolaBlanca!=NULL)
        bolaBlanca->draw();

    for (int i=0; i<listaConjuntBoles.size(); i++) {
        listaConjuntBoles[i]->draw();
    }

}



