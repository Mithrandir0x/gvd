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
    iniCamera(false, vpa, vph);//crea la camera en primera persona.
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
       camGeneral.piram.proj = PARALLELA;
       camGeneral.ini(a, h, capsaMinima);

       camGeneral.vs.obs = vec4(0.0, 20.0, 0.0, 1.0);     
       point4 vrp = point4(0.0, 0.0, 0.0, 1.0);
       setVRPCamera(true, vrp);
       setDCamera(true, 20.0);
       setAnglesCamera(true, -90.0, 0.0, 0.0);

       camGeneral.CalculWindow(capsaMinima);       
       camGeneral.piram.dant = 0.1;
       camGeneral.piram.dpost = 30.0;
       camGeneral.CalculaMatriuProjection();
   }else{
       camFirstP.piram.proj = PERSPECTIVA;
       camFirstP.ini(a, h, capsaMinima);

       camFirstP.vs.vup = vec4(0.0, 1.0, 0.0, 0.0);
       camFirstP.vs.obs = vec4(0.0, 0.0307474, 0.6, 1.0);  
       point4 vrp = point4(0.0, 0.0307474, 0.5, 1.0);
       setVRPCamera(false, vrp);  
       setDCamera(false, 1.3);

       camFirstP.wd.pmin.x = -0.1255;
       camFirstP.wd.pmin.y = -0.0891265;
       camFirstP.wd.a = 0.251;
       camFirstP.wd.h = 0.188253;
       camFirstP.piram.dant = 0.34;
       camFirstP.piram.dpost= 1.7;
       camFirstP.CalculaMatriuProjection();
   }
}


void Escena::setAnglesCamera(bool camGen, float angX, float angY, float angZ){
    if(camGen == true){
        camGeneral.vs.angx = angX;
        camGeneral.vs.angy = angY;
        camGeneral.vs.angz = angZ;
        vec3 vu = camGeneral.CalculVup(camGeneral.vs.angx, camGeneral.vs.angy, camGeneral.vs.angz);
        camGeneral.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);
        camGeneral.vs.obs = camGeneral.CalculObs(camGeneral.vs.vrp, camGeneral.piram.d, camGeneral.vs.angx, camGeneral.vs.angy);
        camGeneral.CalculaMatriuModelView();
    }else{
        camFirstP.vs.angx = angX;
        camFirstP.vs.angy = angY;
        camFirstP.vs.angz = angZ;
        vec3 vu = camFirstP.CalculVup(camFirstP.vs.angx, camFirstP.vs.angy, camFirstP.vs.angz);
        camFirstP.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);
        camFirstP.vs.obs = camFirstP.CalculObs(camFirstP.vs.vrp,camFirstP.piram.d,camFirstP.vs.angx,camFirstP.vs.angy);
        camFirstP.CalculaMatriuModelView();
    }


}

void Escena::setVRPCamera(bool camGen, point4 vrp){
    if(camGen == true){
         camGeneral.vs.vrp = vrp;
         camGeneral.CalculaMatriuModelView();
     }else{
         camFirstP.vs.vrp = vrp;
         camFirstP.CalculaMatriuModelView();
     }
}

void Escena::setWindowCamera(bool camGen, bool retallat, Capsa2D window){
    if(camGen == true){
        camGeneral.wd = window;
        if(retallat == true){
            camGeneral.CalculWindowAmbRetallat();
        }
        camGeneral.AjustaAspectRatioWd();//amplia el window per tal que el seu aspect ratio sigui igual al del viewport
        camGeneral.CalculaMatriuProjection();
    }else{
        camFirstP.wd = window;
        camFirstP.CalculAngleOberturaHoritzontal();
        camFirstP.CalculAngleOberturaVertical();
        if(retallat == true){
            camFirstP.CalculWindowAmbRetallat();
        }
        camFirstP.AjustaAspectRatioWd();//amplia el window per tal que el seu aspect ratio sigui igual al del viewport
        camFirstP.CalculaMatriuProjection();
    }
}

void Escena::setDCamera(bool camGen, float d){
    if(camGen == true){
        camGeneral.piram.d = d;
    }else{
        camFirstP.piram.d = d;
        camFirstP.CalculaMatriuProjection();
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

void Escena::computeCollisions(Capsa3D cb, Capsa3D cT, vec3 ctrB, vector<Capsa3D> listaCapsasConjuntBoles, QKeyEvent *event){
    double deltaDesplacament = 0.01;
    dzP=deltaDesplacament, dzN=-deltaDesplacament, dxP=deltaDesplacament, dxN=-deltaDesplacament;

    if(cT.pmin.z - cb.pmin.z > - deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dzN) < deltaDesplacament en borde de mesa con z negativos
        dzN = cT.pmin.z - cb.pmin.z;
    if(cT.pmin.z + cT.p - (cb.pmin.z + cb.p) < deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dzP) < deltaDesplacament en borde de mesa con z positivos
        dzP = cT.pmin.z + cT.p - (cb.pmin.z + cb.p);
    if(cT.pmin.x - cb.pmin.x > -deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dxN) < deltaDesplacament en borde de mesa con x negativos
        dxN = cT.pmin.x - cb.pmin.x;
    if(cT.pmin.x + cT.a - (cb.pmin.x + cb.a) < deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dxN) < deltaDesplacament en borde de mesa con x positivos
        dxP = cT.pmin.x + cT.a - (cb.pmin.x + cb.a);   

    for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {//comparamos bola blanca con resto de bolas
        if(abs(ctrB.x - (listaCapsasConjuntBoles[i].pmin.x+listaCapsasConjuntBoles[i].a/2.)) < 0.0615
                && abs(ctrB.z  - (listaCapsasConjuntBoles[i].pmin.z+listaCapsasConjuntBoles[i].p/2.)) < 0.0615){

            if(event->key() == Qt::Key_Up){
                if(ctrB.z - (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.)<= 0.0){//si bola blanca con menor z que la i no hay limitacion
                    dzN = -deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z > -deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                        dzN = listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z;
                        if(dzN > -0.002)dzN = 0.0;
                    }
                }
            }

            if(event->key() == Qt::Key_Down){
                if(ctrB.z- (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.) >= 0.0){//si bola blanca con mayor z que la i no hay limitacion
                    dzP = deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.z - (cb.pmin.z + cb.p) < deltaDesplacament ){//si distancia menor que deltaDesplacament hay limitacion
                        dzP = listaCapsasConjuntBoles[i].pmin.z - (cb.pmin.z + cb.p);
                        if(dzP < 0.002)dzP = 0.0;
                    }
                }
            }

            if(event->key() == Qt::Key_Left){
                if(ctrB.x- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.)<= 0.0){//si bola blanca a la izquierda no hay limitacion
                    dxN = -deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x > -deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                        dxN = listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x;
                        if(dxN > -0.002)dxN = 0.0;
                    }
                }
            }

            if(event->key() == Qt::Key_Right){
                if(ctrB.x- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.) >= 0.0){//si bola blanca a la derecha no hay limitacion
                    dxP = deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x - (cb.pmin.x + cb.a) < deltaDesplacament ){//si distancia menor que deltaDesplacament hay limitacion
                        dxP = listaCapsasConjuntBoles[i].pmin.x - (cb.pmin.x + cb.a);
                        if(dxP < 0.002)dxP = 0.0;
                    }
                }
            }

        }
    }
}


void Escena::draw(bool cameraActual) {

    if (taulaBillar!=NULL){
        taulaBillar->toGPU(pr);
        cam2GPU(cameraActual);
        taulaBillar->draw();
    }

    if (plaBase!=NULL){
        plaBase->texture->bind(0);
        plaBase->toGPU(pr);
        cam2GPU(cameraActual);
        plaBase->draw();
    }

    if (bolaBlanca!=NULL){
        bolaBlanca->texture->bind(0);
        bolaBlanca->toGPU(pr);
        cam2GPU(cameraActual);
        bolaBlanca->draw();
    }

    if (conjuntBoles!=NULL){
        for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
            conjuntBoles->listaConjuntBoles[i]->texture->bind(0);
            conjuntBoles->listaConjuntBoles[i]->toGPU(pr);
            cam2GPU(cameraActual);
            conjuntBoles->listaConjuntBoles[i]->draw();
            }
    }

}

void Escena::cam2GPU(bool cameraActual){
    if(cameraActual == true){
        camGeneral.toGPU(pr);
    }else{
        camFirstP.toGPU(pr);
    }
}

void Escena::actualizaMatr(bool cameraActual){
    Capsa3D c;

    if(cameraActual == true){
        //camGeneral.CalculaMatriuModelView();
        camGeneral.CalculaMatriuProjection();
    }else{
        camFirstP.CalculaMatriuModelView();
        camFirstP.CalculaMatriuProjection();
    }
}



