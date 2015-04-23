#include "escena.h"

Escena::Escena(int vpa, int vph)
{
    //std::cout<<"Escena::Escena"<<std::endl;
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0.0; capsaMinima.pmin[1] = 0.0; capsaMinima.pmin[2]=0.0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    taulaBillar = NULL;
    plaBase = NULL;
    bolaBlanca = NULL;
    conjuntBoles = NULL;

    iniCamera(true, vpa, vph);//crea la camera general. a y h del vp obtenidos en glWidget
}

Escena::~Escena()
{
    if (taulaBillar!=NULL)
       delete this->taulaBillar;
    if (plaBase!=NULL)
       delete this->plaBase;
    if (bolaBlanca!=NULL)
       delete this->bolaBlanca;
    if (conjuntBoles!=NULL)
       delete this->conjuntBoles;
}

void Escena::iniCamera(bool camGen, int a, int h){
   if(camGen == true){
       a = 600;//inicialmente 640x480 y
       h = 600; //al usarse Camera::AjustaAspectRatioWd no queda bien
       camGeneral.ini(a, h, capsaMinima);
       camGeneral.vs.obs = vec4(0.0, 20.0, 0.0, 1.0);
       setDCamera(true, 20.0);
       camGeneral.piram.dant = 10.0;
       camGeneral.piram.dpost = 30.0;

       setVRPCamera(true, (0.0, 0.0, 0.0, 1.0));
       //ver Camera::CalculVup
       //un giro de +90 apunta vup a z+
       //un giro de +90 apunta el vector forward de la camara a -x
       //un giro de +90 apunta vup a x-
       setAnglesCamera(true, -90.0, 0.0, 0.0);
       vec3 vu = camGeneral.CalculVup(camGeneral.vs.angx, camGeneral.vs.angy, camGeneral.vs.angz);
       camGeneral.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);

       camGeneral.CalculaMatriuModelView();
       camGeneral.CalculWindow(capsaMinima);
       camGeneral.CalculaMatriuProjection();
   }else{
       //inicializar camera en primera persona
   }
}

void Escena::setAnglesCamera(bool camGen, float angX, float angY, float angZ){
    if(camGen == true){
        camGeneral.vs.angx = angX;
        camGeneral.vs.angy = angY;
        camGeneral.vs.angz = angZ;
    }
}

void Escena::setVRPCamera(bool camGen, point4 vrp){
    if(camGen == true){
        camGeneral.vs.vrp = vrp;
    }
}

void Escena::setWindowCamera(bool camGen, bool retallat, Capsa2D window){
    if(camGen == true){
        camGeneral.wd = window;
        camGeneral.CalculAngleOberturaHoritzontal();
        camGeneral.CalculAngleOberturaVertical();
        if(retallat == true){
            camGeneral.CalculWindowAmbRetallat();
        }
        //camGeneral.AmpliaWindow(0.15);   //0.15 aumenta tamaño window un 15% (disminuye imagen)
        camGeneral.AjustaAspectRatioWd();//amplia el window per tal que el seu aspect ratio sigui igual al del viewport
        camGeneral.CalculaMatriuProjection();
    }
}

void Escena::setDCamera(bool camGen, float d){
    if(camGen == true){
        camGeneral.piram.d = d;
    }
}



void Escena::addObjecte(Objecte *obj) {
    if (dynamic_cast<TaulaBillar*>(obj)){
        this->taulaBillar = (TaulaBillar*)obj;
        listaObjectes.push_back(taulaBillar);
    } else if (dynamic_cast<PlaBase*>(obj)){
        this->plaBase = (PlaBase*)obj;
        listaObjectes.push_back(plaBase);
    }else if (dynamic_cast<Bola*>(obj)){
        this->bolaBlanca= (Bola*)obj;
        listaObjectes.push_back(bolaBlanca);
    }
}


void Escena::CapsaMinCont3DEscena()
{
    // Metode a implementar
    Capsa3D c;
    vec3 pmax;

    capsaMinima.pmin[0]=INFINITY;
    capsaMinima.pmin[1]=INFINITY;
    capsaMinima.pmin[2]=INFINITY;
    pmax[0] = -INFINITY;
    pmax[1] = -INFINITY;
    pmax[2] = -INFINITY;

    for (int i=0; i<listaObjectes.size(); i++) {
        c = listaObjectes[i]->calculCapsa3D();

        if (capsaMinima.pmin[0]>c.pmin[0]) capsaMinima.pmin[0] = c.pmin[0];
        if (capsaMinima.pmin[1]>c.pmin[1]) capsaMinima.pmin[1] = c.pmin[1];
        if (capsaMinima.pmin[2]>c.pmin[2]) capsaMinima.pmin[2] = c.pmin[2];
        if (pmax[0]<c.pmin[0]+c.a) pmax[0] = c.pmin[0]+c.a;
        if (pmax[1]<c.pmin[1]+c.h) pmax[1] = c.pmin[1]+c.h;
        if (pmax[2]<c.pmin[2]+c.p) pmax[2] = c.pmin[2]+c.p;
    }
    capsaMinima.a = pmax[0]-capsaMinima.pmin[0];
    capsaMinima.h = pmax[1]-capsaMinima.pmin[1];
    capsaMinima.p = pmax[2]-capsaMinima.pmin[2];
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

    this->CapsaMinCont3DEscena();

    float xTrasl = capsaMinima.pmin.x + capsaMinima.a/2.;//calculo del centro de la caja de la escena
    float yTrasl = capsaMinima.pmin.y + capsaMinima.h/2.;
    float zTrasl = capsaMinima.pmin.z + capsaMinima.p/2.;

    mat4 maux = Translate(xTrasl, yTrasl, zTrasl) * m * Translate(-xTrasl, -yTrasl, -zTrasl);

    this->aplicaTG(maux);
}

void Escena::draw() {

    if (taulaBillar!=NULL){
        taulaBillar->toGPU(pr);
        camGeneral.toGPU(pr);
        taulaBillar->draw();
    }

    if (plaBase!=NULL){
        plaBase->texture->bind(0);
        plaBase->toGPU(pr);
        camGeneral.toGPU(pr);
        plaBase->draw();
    }

    if (bolaBlanca!=NULL){
        bolaBlanca->texture->bind(0);
        bolaBlanca->toGPU(pr);
        camGeneral.toGPU(pr);
        bolaBlanca->draw();
    }

    if (conjuntBoles!=NULL){
        for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
            conjuntBoles->listaConjuntBoles[i]->texture->bind(0);
            conjuntBoles->listaConjuntBoles[i]->toGPU(pr);
            camGeneral.toGPU(pr);
            conjuntBoles->listaConjuntBoles[i]->draw();
            }
    }

}

void Escena::tuneCamera(bool retallat, bool centrat, QGLShaderProgram *program){
    double r;


    camGeneral.vs.angx = 0.0;
    camGeneral.vs.angy = 0.0;
    camGeneral.vs.angz = 0.0;

    CapsaMinCont3DEscena();
    camGeneral.vs.vrp[0] = capsaMinima.pmin[0]+(capsaMinima.a/2.0);
    camGeneral.vs.vrp[1] = capsaMinima.pmin[1]+(capsaMinima.h/2.0);
    camGeneral.vs.vrp[2] = capsaMinima.pmin[2]+(capsaMinima.p/2.0);

    //camGeneral.vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
    camGeneral.vs.obs = vec4(0.0, 20.0, 0.0, 1.0);
    camGeneral.vs.vup = vec4(0.0, 0.0, -1.0, 0.0);

    camGeneral.piram.proj = PARALLELA;
    camGeneral.piram.d = 20.0;
    camGeneral.piram.dant = 10.0;
    camGeneral.piram.dpost = 30.0;

    camGeneral.CalculaMatriuModelView();

    camGeneral.CalculWindow(capsaMinima);

    /*camGeneral.wd.a = 3.45;//al disminuir reduce al ancho de los objetos y los lleva a la derecha
                           //al aumentar reduce al ancho de los objetos y los lleva a la izquierda
    camGeneral.wd.h = 3.45;//al disminuir aumenta la profundidad de los objetos y los lleva hacia arriba
                           //al aumentar disminuye la profundidad de los objetos y los lleva hacia abajo
    camGeneral.wd.pmin[0] = -1.725;//al aumentar mueve los objetos a la izquierda
    camGeneral.wd.pmin[1] = -1.725;//al aumentar mueve los objetos hacia abajo*/

    camGeneral.CalculaMatriuProjection();

    /*camGeneral.vp.a = 640;//al aumentar, aumenta la profundidad de la mesa
    camGeneral.vp.h = 640;//al aumentar, aumenta la anchura de la mesa
    camGeneral.vp.pmin[0] = 0;//no parece afectar la presentacion
    camGeneral.vp.pmin[1] = 0;//no parece afectar la presentacion*/
}


