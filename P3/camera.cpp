#include "camera.h"

Camera::Camera()
{
    //std::cout<<"Camera::Camera"<<std::endl;
    vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
    vs.vup = vec4(0.0, 1.0, 0.0, 0.0);
    vs.obs = vec4(0.0, 20.0, 0.0, 1.0);

    vs.angx = 0.0;
    vs.angy = 0.0;
    vs.angz = 0.0;//con los 3 angulos a cero vup apunta a (0,1,0)

    setViewport(0, 0, 600, 600);

    piram.proj = PARALLELA;
    piram.d = 2;
}

void Camera::ini(int a, int h, Capsa3D capsaMinima)
{
    vs.vrp[0] = capsaMinima.pmin.x + capsaMinima.a/2;
    vs.vrp[1] = capsaMinima.pmin.y + capsaMinima.h/2;
    vs.vrp[2] = capsaMinima.pmin.z + capsaMinima.p/2;

    vp.a = a;
    vp.h = h;
    vp.pmin[0] = 0;
    vp.pmin[1] = 0;

}

// Suposa que les dades d'obs, vrp i vup son correctes en la camera
// angx, angy, angles de gir del sistema de coords obser
void Camera::CalculaMatriuModelView()
{
    modView = identity();

    vec4 eye = vs.obs; //se supone correcto
    vec4 at = vs.vrp;  //se supone correcto
    vec4 up = vs.vup;  //se supone correcto

    this->modView = LookAt(eye, at, up);
}

void Camera::CalculaMatriuProjection()
{
    //se suopne que wd, dant y dpost son correctos
    proj = identity();

    if(piram.proj == PARALLELA){
       proj = Ortho(wd.pmin.x, wd.pmin.x+wd.a, wd.pmin.y, wd.pmin.y+wd.h, piram.dant, piram.dpost);
    }

    if(piram.proj == PERSPECTIVA){
       proj = Frustum(wd.pmin.x, wd.pmin.x+wd.a, wd.pmin.y, wd.pmin.y+wd.h, piram.dant, piram.dpost);
    }
}


void Camera::CalculWindow( Capsa3D c)
{
    mat4 MDP;
    vec4  vaux[8], vauxMod[8];

    //modView = LookAt(vs.obs, vs.vrp, vs.vup);

    if (piram.proj==PERSPECTIVA) {
        CreaMatDp(MDP); // crea la matriu de deformacio perspectiva
        modView = MDP * modView;
    }

    VertexCapsa3D(c, vaux);//se pasa la capsa y obtenemos sus vertices

    for(int i=0; i<8; i++) {
        vauxMod[i]= modView * vaux[i];
    }

    wd = CapsaMinCont2DXYVert(vauxMod, 8);
    AjustaAspectRatioWd();//amplia el window per tal que el seu aspect ratio sigui igual al del viewport
}

void Camera::setViewport(int x, int y, int a, int h)
{
    vp.pmin[0] = x;
    vp.pmin[1] = y;
    vp.a = a;
    vp.h = h;
}

void Camera::toGPU(QGLShaderProgram *program)
{
    this->setProjectionToGPU(program, this->proj);
    this->setModelViewToGPU(program, this->modView);
}

void Camera::setModelViewToGPU(QGLShaderProgram *program, mat4 m)
{
    model_view = program->uniformLocation("model_view");
    glUniformMatrix4fv( model_view, 1, GL_TRUE, m );

}

void Camera::setProjectionToGPU(QGLShaderProgram *program, mat4 p)
{
    projection = program->uniformLocation("projection");
    glUniformMatrix4fv(projection,1,GL_TRUE, p);
}

void  Camera::AmpliaWindow(double r)
{
    // Pre r = 1.5 => amplia a y h 150% equivalente a un zoom out
    //para hacer zoom in usar r negativo
    double na, da;

    na  = wd.a * (1.0 + r);
    da = na - wd.a;
    wd.pmin[0] = wd.pmin[0] - 0.5*da;
    wd.a  = na;
    na  = wd.h * (1.0 + r);
    da = na - wd.h;
    wd.pmin[1] = wd.pmin[1] - 0.5*da;
    wd.h  = na;
}



// procediment que calcula el semi angle d'obertura vertical
// a partir de l'alcada total del window (h) i la distancia
// a l'observador

void Camera::CalculAngleOberturaVertical()
{
    piram.alfav =  180.0 * atan2(wd.h/2.0, piram.d)/PI;

}

// procediment que calcula el semi angle d'obertura horitzontal
// a partir de l'amplada total del window (a) i la distancia
// a l'observador

void Camera::CalculAngleOberturaHoritzontal()
{
    piram.alfah =  180.0 * atan2(wd.a/2.0, piram.d)/PI;

}

void  Camera::CalculWindowAmbRetallat()
{
    Capsa2D c;

    if (piram.proj == PARALLELA) {
      /* Projeccio paral.lela:
           el window ve donat: amplada o alcada o tots dos. */

      if( fabs(wd.h) <EPS ) {
        c.a = wd.a;
        c.h = ((float)(vp.h)/ (float)(vp.a) ) * wd.a;
      }
      else {
        c.h = wd.h;
        c.a = ((float)(vp.a)/ (float)(vp.h) ) * wd.h;
      }
    }
    else {

      /* Projeccio perspectiva:
           el window ve donat pels angles d'obertura de la camera: el
           vertical, l'horitzontal o tots dos.  */

      if( fabs(piram.alfav) <EPS ) {
        c.a = 2.0  * piram.d * tan( PI*piram.alfah/180.);
        c.h = ((float)(vp.h)/ (float)(vp.a) ) * c.a;
      }
      else {
        if (fabs(piram.alfah) <EPS ) {
          c.h = 2.0  * piram.d * tan( PI*piram.alfav/180.);
          c.a = ((float)(vp.a)/ (float)(vp.h) ) * c.h;
        }
        else {
          c.a = 2.0  * piram.d * tan( PI*piram.alfah/180.);
          c.h = 2.0  * piram.d * tan( PI*piram.alfav/180.);
        }
      }
    }
    c.pmin[0] = -0.5 * c.a;
    c.pmin[1] = -0.5 * c.h;

    wd.pmin[0] = c.pmin[0];
    wd.pmin[1] = c.pmin[1];
    wd.a = c.a;
    wd.h = c.h;
}



// Donat un window i un viewport, amplia el window per tal que el seu
// aspect ratio sigui igual al del viewport

void    Camera::AjustaAspectRatioWd()
{
    double arvp, arwd;

    arvp = ((double) vp.h)/((double)(vp.a));
    arwd = wd.h/wd.a;
    if(arwd > arvp) {
        //wd.a = wd.h/arvp;
        wd.a = wd.h * 1.008;
    }
    else if (arwd <arvp) {
        wd.h = wd.a *arvp;

    }
    wd.pmin[0] = -0.5*wd.a;
    wd.pmin[1] = -0.5*wd.h;
}



// Accio que crea la matriu de deformacio perspectiva

void Camera::CreaMatDp(mat4 &MDP)
{
    MDP = identity();
    MDP[3][2] = -1/piram.d;
}


//calcula el wd
Capsa2D  Camera::CapsaMinCont2DXYVert( vec4 *v, int nv)
{
    Capsa2D c;
    vec2    pmin, pmax;
    int     i;

    pmin[0] = v[0][0];       pmin[1] = v[0][1];
    pmax[0] = v[0][0];       pmax[1] = v[0][1];
    for(i=1; i<nv; i++) {
      if(v[i][0] <pmin[0])
        pmin[0] = v[i][0];
      if(v[i][1] <pmin[1])
        pmin[1] = v[i][1];
      if(v[i][0] >pmax[0])
        pmax[0] = v[i][0];
      if(v[i][1] >pmax[1])
        pmax[1] = v[i][1];
    }

    c.a = pmax[0]-pmin[0] + 0.5 ;
    c.h = pmax[1]-pmin[1] + 0.5;
    c.pmin[0] = -c.a/2.0;
    c.pmin[1] = -c.h/2.0;
    return c;
}



//   Calcul del observador:
//   obs es el vrp translladat en la direccio del vector de visio  d'una
//   distancia d = distancia (obs, vrp) si la projeccio es perspectiva
//   i d'una distancia prou gran si la projeccio es paral.lela

vec4 Camera::CalculObs(vec4 vrp, double d, double angx, double angy)
{
    vec4 obs2;
    vec3 v;
    double norma;

    /* Calcul del vector de visio a partir dels angles */
    //angx es la latitud y angy la longitud respecto al eje z

    v[0] = sin (PI*angy/180.) * cos (PI*angx/180.);
    v[1]= - sin (PI*angx/180.);
    v[2] = cos (PI*angy/180.) * cos (PI*angx/180.);

    norma = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);

    v[0] = v[0]/norma;
    v[1] = v[1]/norma;
    v[2] = v[2]/norma;


    obs2[0] = vrp[0] + v[0] *d;
    obs2[1] = vrp[1] + v[1] *d;
    obs2[2] = vrp[2] + v[2] *d;
    obs2[3] = 1.0;
    return(obs2);

}

vec3 Camera::CalculVup(double angx, double angy, double angz) // angx, angy, angz, angles de gir del sistema de coords obser
{
    vec3 v;
    /*int  x, y;

    x = 1.0;
    y = 1.0;

    if (cos(PI*angx/180.)<0.0) y = -1.0;

    if (cos(PI*angy/180.)<0.0) x = -1.0;


    v[0] = x*sin (-PI*angz/180.);
    v[1] = y*cos( -PI*angz/180.);
    v[2] = 0.0;*/

    double sx = sin(PI*angx/180.); //se supone Rz * Ry * Rx
    double cx = cos(PI*angx/180.); //ver http://www.songho.ca/opengl/gl_anglestoaxes.html
    double sy = sin(PI*angy/180.);
    double cy = cos(PI*angy/180.);
    double sz = sin(PI*angz/180.);
    double cz = cos(PI*angz/180.);

    //v = vec3(-sz*cx+cz*sy*sx, cz*cx+sz*sy*sx, cy*sx);//formula completa
    v = vec3(sx*sy, cx, sx*cy);//suponiendo angz = 0

    return(v);

}

void Camera::VertexCapsa3D(Capsa3D capsaMinima, vec4 vaux[8])
{
    vec3 ptfi;

    ptfi[0] = capsaMinima.pmin[0]+capsaMinima.a;
    ptfi[1] = capsaMinima.pmin[1]+capsaMinima.h;
    ptfi[2] = capsaMinima.pmin[2]+capsaMinima.p;

    vaux[0] = vec4(capsaMinima.pmin[0], capsaMinima.pmin[1], capsaMinima.pmin[2], 1.0);
    vaux[1] = vec4(capsaMinima.pmin[0], capsaMinima.pmin[1], ptfi[2], 1.0);
    vaux[2] = vec4(capsaMinima.pmin[0],  ptfi[1], capsaMinima.pmin[2],1.0);
    vaux[3] = vec4(capsaMinima.pmin[0], ptfi[1], ptfi[2], 1.0);
    vaux[4] = vec4(ptfi[0],capsaMinima.pmin[1], capsaMinima.pmin[2],1.0);
    vaux[5] = vec4(ptfi[0], capsaMinima.pmin[1], ptfi[2],1.0);
    vaux[6] = vec4(ptfi[0], ptfi[1], capsaMinima.pmin[2], 1.0);
    vaux[7] = vec4(ptfi[0], ptfi[1], ptfi[2], 1.0);
}


void Camera::PrintCamera(){

    std::cout<<"\nangx: "<<vs.angx<<", angy: "<<vs.angy<<", angz: "<<vs.angz<<std::endl;
    std::cout<<"vrp[0]: "<<vs.vrp[0]<<", vrp[1]: "<<vs.vrp[1]<<", vrp[2]: "<<vs.vrp[2]<<std::endl;
    std::cout<<"obs[0]: "<<vs.obs[0]<<", obs[1]: "<<vs.obs[1]<<", obs[2]: "<<vs.obs[2]<<std::endl;
    std::cout<<"vup[0]: "<<vs.vup[0]<<", vup[1]: "<<vs.vup[1]<<", vup[2]: "<<vs.vup[2]<<std::endl;
    std::cout<<"d: "<<piram.d<<", dant: "<<piram.dant<<", dpost: "<<piram.dpost<<std::endl;
    std::cout<<"wd.pmin[0]: "<<wd.pmin[0]<<", wd.pmin[1]: "<<wd.pmin[1]<<", wd.a: "<<wd.a<<", wd.h: "<<wd.h<<std::endl;
    std::cout<<"vp.pmin[0]: "<<vp.pmin[0]<<", vp.pmin[1]: "<<vp.pmin[1]<<", vp.a: "<<vp.a<<", vp.h: "<<vp.h<<std::endl;
}


