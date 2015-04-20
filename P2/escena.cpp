#include "escena.h"

Escena::Escena()
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    taulaBillar = NULL;
    plaBase = NULL;
    bolaBlanca = NULL;
    conjuntBoles = NULL;

    // Inicialització de la càmera
    camGeneral = new Camera();
}

Escena::~Escena()
{
    // Cal anar fent delete dels objectes que se'l hagi fet new
   delete this->taulaBillar;
   delete this->plaBase;
   delete this->bolaBlanca;
   delete this->conjuntBoles;
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
    if (conjuntBoles!=NULL){
            for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
                    conjuntBoles->listaConjuntBoles[i]->aplicaTG(m);
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
    if (conjuntBoles!=NULL){
            for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
                    conjuntBoles->listaConjuntBoles[i]->aplicaTGCentrat(m);
            };
    }

}

void Escena::draw(QGLShaderProgram *pr) {

    // Metode a modificar

    if (taulaBillar!=NULL)
            taulaBillar->draw();

    if (plaBase!=NULL){
        plaBase->texture->bind(0);
        plaBase->toGPU(pr);
        plaBase->draw();
    }

    if (bolaBlanca!=NULL){
        bolaBlanca->texture->bind(0);
        bolaBlanca->toGPU(pr);
        bolaBlanca->draw();
    }

    if (conjuntBoles!=NULL){
        for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
            conjuntBoles->listaConjuntBoles[i]->texture->bind(0);
            conjuntBoles->listaConjuntBoles[i]->toGPU(pr);
            conjuntBoles->listaConjuntBoles[i]->draw();
            }
    }

}

/*
 * Mètodes introduïts pel sistema de càmera
 */
void Escena::SetZenitCamera()
{
    // Situem "l'objectiu" que mira la càmera a (0, 0, 0), a una distància de 20
    // respecte l'eix de la Y.
    camGeneral.vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
    camGeneral.vs.vup = vec4(0.0, 0.0, 1.0, 1.0);
    camGeneral.vs.obs = camGeneral->CalculObs(camGeneral->vs.vrp, 20, 0, 90.0);
}

void Escena::iniCamera(bool isCamGeneral) {
    if ( isCamGeneral ) {
        Capsa3D capsaMinima = CapsaMinCont3DEscena();
        camGeneral.ini(screenSize[0], screenSize[1], capsaMinima);
        SetZenitCamera();
    } else {
    }
}
