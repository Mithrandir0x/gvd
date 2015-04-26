# Abstract

Per seguir aquest informe de pràctica s'ha de tenir en compte que s'ha partit
des de la implementació existent de la primera pràctica.

# Correccions sobre la pràctica 1

Aquesta primera part es dedicarà a descriure els canvis introduïts fets arrel
de les observacions donades per la primera pràctica.

## `PlaBase`

- Canviar la manera en que es defineixen les coordenades inicials del pla base.
  En comptes que estiguin en el constructor, haurien d'estar definides a través
  de métodes GET/SET i cridades des d'`Escena` o des de `GlWidget`.

> S'han introduit uns canvis a la clase:
>
> ##### Nova declaració del constructor a la capçalera `plabase.h`:
>
> ```c
> PlaBase(point4 v0, point4 v1, point4 v2, point4 v3, color4 cv0, color4 cv1, color4 cv2, color4 cv3);
> ```
>
> ##### Nova implementació del constructor al mòdul `plabase.cpp`:
> 
> ```c
> PlaBase::PlaBase(point4 v0, point4 v1, point4 v2, point4 v3, color4 cv0, color4 cv1, color4 cv2, color4 cv3) : Objecte(NumVertices)
> {
> 
>     vertices[0] = v0;
>     vertices[1] = v1;
>     vertices[2] = v2;
>     vertices[3] = v3;
> 
>     vertex_colors[0] = cv0; //white
>     vertex_colors[1] = cv1; //red
>     vertex_colors[2] = cv2; //yellow
>     vertex_colors[3] = cv3; //green
> 
> 
>     make();
>     capsa = calculCapsa3D();
> 
>     /*double escalap = 2.0 / capsa.p;//para escalar la dimension maxima(profundidad) a 2
>     double escalaa = 1.08821869 / capsa.a;//para que coincida con la mesa de billar
> 
>     mat4 m = Scale(escalaa, 1.0, escalap)*Translate(-(capsa.pmin.x + capsa.a/2.), -(capsa.pmin.y + capsa.h/2.), -(capsa.pmin.z + capsa.p/2.));
>     aplicaTG(m);
>     capsa = calculCapsa3D();*/
> }
> ```
>
> Després s'ha canviat la manera d'instanciar el pla base a l'escena des de `GlWidget`:
>
> ```c
> void GLWidget::newPlaBase()
> {
>     cout << "Creating new pla base" << endl;
>     // Metode que crea un objecte PlaBase poligon amb el punt central al (0,0,0) i perpendicular a Y=0
>     point4 v0  = point4( 10.0, 0.0, 10.0, 1.0 );
>     point4 v1  = point4( 10.0, 0.0,-10.0, 1.0 );
>     point4 v2  = point4(-10.0, 0.0,-10.0, 1.0 );
>     point4 v3  = point4(-10.0, 0.0, 10.0, 1.0 );
> 
>     color4 cv0 = color4( 1.0, 1.0, 1.0, 1.0 ); //white
>     color4 cv1  = color4( 1.0, 0.0, 0.0, 1.0 ); //red
>     color4 cv2  = color4( 1.0, 1.0, 0.0, 1.0 ); //yellow
>     color4 cv3  = color4( 0.0, 1.0, 0.0, 1.0 ); //green
> 
>     PlaBase *plaBase = new PlaBase(v0, v1, v2, v3, cv0, cv1, cv2, cv3);
>     newObjecte(plaBase);
> }
> ```

## `Bola`

- Millorar el codi de generació de triangles de la bola de la funció `Bola::triangle`.

> Per corregir aquesta part, hem modificat el mètode quedant de la següent manera:
>
> ```c
> void Bola::triangle(const vec4& a, const vec4& b, const vec4& c )
> {
>     double u, v;
> 
>     vec4 vertArr[3] = {a, b, c};
> 
>     for(int i =0; i<3;i++){
>         points[Index] = vertArr[i];
>         colors[Index] = this->color;
>         u = 0.5 + atan2(-points[Index].z, -points[Index].x)/(2*M_PI);
>         v = 0.5 - asin(-points[Index].y)/M_PI;
>         if(u < 0.0)u=0.0;
>         if(v < 0.0)v=0.0;
>         if(u > 1.0)u=1.0;
>         if(v > 1.0)v=1.0;
>         vertexsTextura[Index] = vec2(u , v);
>         Index++;
>     }
> }
> ```

## `GlWidget`

- Descomentar `GLWidget::adaptaObjecteTamanyWidget`, i verificar que al treure
  els escalats dels constructors funciona correctament l'adaptació a la capsa
  (-1,1).

> En primer lloc, s'ha comentat l'escalat i translació que es feien des dels
> constructors de les classes afectades, `PlaBase` i `Bola`.
>
> Després, s'ha implementat el mètode `GLWidget::adaptaObjecteTamanyWidget`:
>
> ```c
> void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
> {
>     // Metode a implementar
>     Capsa3D capsa;
>     mat4 m;
> 
>     capsa = obj->calculCapsa3D();
>     if (dynamic_cast<TaulaBillar*>(obj)){
>         double aristaMax = 0.0;
> 
>         if(capsa.a > capsa.p){
>             aristaMax=capsa.a;
>         } else{
>             aristaMax=capsa.p;
>         }
>         if(capsa.h > aristaMax){
>             aristaMax=capsa.h;
>         }
> 
>         double escala = 2.0 / aristaMax;
>         mat4 m = Scale(escala, escala, escala);
>         obj->aplicaTG(m);//la mayor arista de la caja contenedora de la mesa es 2.0
> 
>         capsa = obj->calculCapsa3D();
> 
>         m = Translate(-(capsa.pmin.x + capsa.a / 2.), -(capsa.pmin.y + capsa.h), -(capsa.pmin.z + capsa.p / 2.));
>         obj->aplicaTG(m);//deja el centro de la mesa(x,z) en el origen de coordenadas y el tope en el plano y=0, con arista maxima 2.0
>         //capsa = obj->calculCapsa3D();
> 
>     }else if (dynamic_cast<Bola*>(obj)){
>             m = Translate(0.0,  -capsa.pmin.y, 0.0);//la base de las 16 bolas quedan en y = 0
>             obj->aplicaTG(m);
>     }else if (dynamic_cast<PlaBase*>(obj)){
>             mat4 m = Scale(2.0/(1.8379*capsa.a), 1.0, 2.0/capsa.p)*Translate(-(capsa.pmin.x + capsa.a/2.), -(capsa.pmin.y + capsa.h/2.), -(capsa.pmin.z + capsa.p/2.));
>             obj->aplicaTG(m);
>     }
>     //capsa = obj->calculCapsa3D();
> }
> ```
>
> Aquest canvi es va introduir inicialment a partir del feedback donat, però
> al llarg de la pràctica, aquesta implementació desapareixerà degut a que ja
> no és necessari donat que la càmera fa aquesta tasca.

## `Escena`

- A `Escena::aplicaTGCentrat`, s'ha d'aplicar la transformació de la matriu
  respecte el centre de la capsa contenidora de l'escena.

> S'ha reimplementat el mètode per fer el que toca:
>
> ```c
> void Escena::aplicaTGCentrat(mat4 m) {
> 
>     this->CapsaMinCont3DEscena();
> 
>     float xTrasl = capsaMinima.pmin.x + capsaMinima.a/2.;//calculo del centro de la caja de la escena
>     float yTrasl = capsaMinima.pmin.y + capsaMinima.h/2.;
>     float zTrasl = capsaMinima.pmin.z + capsaMinima.p/2.;
> 
>     mat4 maux = Translate(xTrasl, yTrasl, zTrasl) * m * Translate(-xTrasl, -yTrasl, -zTrasl);
> 
>     this->aplicaTG(maux);
> }
> ```

- A `Escena::~Escena` s'ha d'evitar destruir objectes nuls.

> Per arreglar aquest problema, hem canviat la implementació del mètode:
>
> ```c
> Escena::~Escena()
> {
>     // Cal anar fent delete dels objectes que se'l hagi fet new
>     if (taulaBillar!=NULL)
>        delete this->taulaBillar;
>     if (plaBase!=NULL)
>        delete this->plaBase;
>     if (bolaBlanca!=NULL)
>        delete this->bolaBlanca;
>     if (conjuntBoles!=NULL)
>        delete this->conjuntBoles;
> }
> ```

- Falta implementar el mètode `CapsaMinCont3DEscena`.

> Et voila:
>
> ```c
> void Escena::CapsaMinCont3DEscena()
> {
>     // Metode a implementar
>     Capsa3D c;
>     vec3 pmax;
> 
>     capsaMinima.pmin[0]=INFINITY;
>     capsaMinima.pmin[1]=INFINITY;
>     capsaMinima.pmin[2]=INFINITY;
>     pmax[0] = -INFINITY;
>     pmax[1] = -INFINITY;
>     pmax[2] = -INFINITY;
> 
>     for (int i=0; i<listaObjectes.size(); i++) {
>         c = listaObjectes[i]->calculCapsa3D();
> 
>         if (capsaMinima.pmin[0]>c.pmin[0]) capsaMinima.pmin[0] = c.pmin[0];
>         if (capsaMinima.pmin[1]>c.pmin[1]) capsaMinima.pmin[1] = c.pmin[1];
>         if (capsaMinima.pmin[2]>c.pmin[2]) capsaMinima.pmin[2] = c.pmin[2];
>         if (pmax[0]<c.pmin[0]+c.a) pmax[0] = c.pmin[0]+c.a;
>         if (pmax[1]<c.pmin[1]+c.h) pmax[1] = c.pmin[1]+c.h;
>         if (pmax[2]<c.pmin[2]+c.p) pmax[2] = c.pmin[2]+c.p;
>     }
>     capsaMinima.a = pmax[0]-capsaMinima.pmin[0];
>     capsaMinima.h = pmax[1]-capsaMinima.pmin[1];
>     capsaMinima.p = pmax[2]-capsaMinima.pmin[2];
> }
> ```

- Ara serà `Escena` qui s'encarregui de gestionar les colisions de les boles,
  en comptes de `GLWidget`. Per això s'ha introduït el mètode `computeCollisions`:

> La signatura del mètode:
>
> ```c
> void Escena::computeCollisions(Capsa3D cb, Capsa3D cT, vec3 ctrB, vector<Capsa3D> listaCapsasConjuntBoles, QKeyEvent *event){
>     // El codi segueix sent el mateix que hi havia a GLWidget
>     // al mètode KeyPressEvent.
> }
> ```
> 
> A més, s'han afegit com a atributs de la clase:
>
> ```c
> // escena.h
> double dzP = 0.01;
> double dzN = -0.01;
> double dxP = 0.01;
> double dxN = -0.01;
> ```

---

# Progrés de la pràctica

En aquesta segona secció es descriurà el codi introduït per integrar la càmera
a l'escena, amb els dos tipus demanats.

## 2 Modificació del Vertex Shader

### 2.1 Definició i ús de la matriu Model-View/Projection en el Vertex Shader

Al fitxer `vshader1.glsl`, s'han afegit les següents variables:

```glsl
uniform mat4 model_view;
uniform mat4 projection;
```

I s'ha canviat la manera amb la que es calcula la posició del vèrtex processat:

```glsl
gl_Position = projection * model_view * vPosition;
gl_Position = gl_Position / gl_Position.w;
```

Quan es multiplica per `model_view`, s'obté les coordenades de càmera.

Quan es multiplica per `projection` el càlcul anterior, s'obté les coordenades
normalitzades.

I per últim, quan es divideix per la quarta ordenada, s'obté les coordenades
homogeneitzades.

## 3 Modificació de la classe càmera

### 3.1 Implementació del pas a la GPU de la matriu M-V/P des de la classe Càmera

S'implementen els següents mètodes dins la clase Camera:

#### `Camera::setModelViewToGPU`

Es demana un registre uniforme per passar la matriu Model-View:

```c
void Camera::setModelViewToGPU(QGLShaderProgram *program, mat4 m)
{
    // CODI A MODIFICAR DURANT LA PRACTICA 2
    model_view = program->uniformLocation("model_view");
    glUniformMatrix4fv( model_view, 1, GL_TRUE, m );
}
```

#### `Camera::setProjectionToGPU`

Es demana un registre uniforme per passar la matriu Projection:

```c
void Camera::setProjectionToGPU(QGLShaderProgram *program, mat4 p)
{
    // CODI A MODIFICAR DURANT LA PRACTICA 2
    projection = program->uniformLocation("projection");
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );
}
```

#### `Camera::toGPU`

Executem els dos mètodes implementats:

```c
void Camera::toGPU(QGLShaderProgram *program)
{
    // CODI A MODIFICAR DURANT LA PRACTICA 2
    setModelViewToGPU(program, modView);
    setProjectionToGPU(program, projection);
}
```

### 3.2 Implementació de diferents utilitats de la classe Camera

S'han d'implementar els mètodes indicats:

#### `Camera::ini`

En primer lloc, es calcula el View Reference Point a partir de la capsa
contenidora passada per paràmetre:

```c
void Camera::ini(int a, int h, Capsa3D capsaMinima)
{
    // Calcul del vrp com el centre de la capsa minima contenedora 3D
    // CAL IMPLEMENTAR
    // CODI A MODIFICAR DURANT LA PRACTICA 2

    vs.vrp[0] = capsaMinima.pmin.x + capsaMinima.a/2;
    vs.vrp[1] = capsaMinima.pmin.y + capsaMinima.h/2;
    vs.vrp[2] = capsaMinima.pmin.z + capsaMinima.p/2;

    vp.a = a;
    vp.h = h;
    vp.pmin[0] = 0;
    vp.pmin[1] = 0;

}
```

#### `Camera::CalculaMatriuModelView`

A partir de com està explicat a la diapositiva 22 de la segona sessió del
tema 3, fem les següents transformacions geomètriques:

```c
void Camera::CalculaMatriuModelView()
{
    // CODI A MODIFICAR DURANT LA PRACTICA 2
    modView = identity();

    vec4 eye = vs.obs;
    vec4 at = vs.vrp;
    vec4 up = vs.vup;

    this->modView = LookAt(eye, at, up);
}
```

#### `Camera::CalculaMatriuProjection`

En funció del tipus de projecció que s'estigui utilitzant, la matriu de
projecció canvia:

```c
void Camera::CalculaMatriuProjection()
{
    // CODI A MODIFICAR DURANT LA PRACTICA 2
    proj = identity();

    if(piram.proj == PARALLELA){
       proj = Ortho(wd.pmin.x, wd.pmin.x+wd.a, wd.pmin.y, wd.pmin.y+wd.h, piram.dant, piram.dpost);
    }

    if(piram.proj == PERSPECTIVA){
       proj = Frustum(wd.pmin.x, wd.pmin.x+wd.a, wd.pmin.y, wd.pmin.y+wd.h, piram.dant, piram.dpost);
    }

}
```

#### `Camera::CalculWindow`

La *window* es calcula amb coordenades de càmera. Per això, s'agafa la capsa
contenidora, i es transforma a coordenades de càmera.

```c
void Camera::CalculWindow(Capsa3D c)
{
   // CODI A MODIFICAR DURANT LA PRACTICA 2

    mat4 MDP;
    vec4  vaux[8], vauxMod[8];
    //wd.pmin.x = -1;
    //wd.pmin.y = -1;

    //wd.a = 2;
    //wd.h = 2;

    CalculaMatriuModelView();

    if (piram.proj==PERSPECTIVA) {
        CreaMatDp(MDP); // crea la matriu de deformacio perspectiva
        modView = MDP * modView;
    }

    VertexCapsa3D(c, vaux);//se pasa la capsa y obtenemos sus vertices

    for(int i=0; i<8; i++) {
        vauxMod[i]= modView * vaux[i];
    }

    wd = CapsaMinCont2DXYVert(vauxMod, 8);
    AmpliaWindow(0.15);      //probar si es necesario
    AjustaAspectRatioWd();//amplia el window per tal que el seu aspect ratio sigui igual al del viewport
}
```

## 4 Modificació de la classe Escena per incloure la càmera general

Abans d'entrar amb els canvis d'aquesta secció, cal explicar una sèrie de canvis
introduïts a la classe `Escena` per acomodar-nos alguns dels canvis posteriors.

S'ha afegit un nou atribut a la classe esmentada, que ens permet guardar tots els
objectes que conté l'escena en una llista:

```c
// escena.h
vector<Objecte*> listaObjectes;
```

En el constructor, `Escena::Escena()`, s'ha afegit la inicialització:

```c
conjuntBoles = NULL;
```

I desprès, quan s'afegia un objecte a l'escena s'ha modificat el mètode
`Escena::addObjecte` per que ho afegir els objectes que s'afegisin a la
llista esmentada:

```c
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
```

Com a la pràctica anterior no s'havia implementat el mètode `CapsaMinCont3DEscena`,
i en vista que segurament en aquesta pràctica ens aniria be tenir-ho a ma, em
afegit la següent implementació:

```c
void Escena::CapsaMinCont3DEscena()
{
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
```

### 4.1 Inclusió d'un nou atribut `camGeneral` a la classe Escena

A `escena.h`, s'ha inclós la següent capsalera:

```c
#include <camera.h>
```

I s'ha afegit el següent atribut públic:

```c
// Camera
Camera camGeneral;
```

S'ha canviat el constructor de l'escena per que rebi per paràmetre l'amplada
i l'alçada de la finestra.

```c
// escena.h
Escena::Escena(int vpa, int vph)
```

La seva implementació també s'ha canviat per que inicialitzi la càmera:

```c
Escena::Escena(int vpa, int vph)
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    taulaBillar = NULL;
    plaBase = NULL;
    bolaBlanca = NULL;
    conjuntBoles = NULL;

    iniCamera(true, vpa, vph);//crea la camera general. a y h del vp obtenidos en glWidget
}
```

### 4.2 Implementació de diferents utilitats referents a la càmera la classe Escena

#### `Escena::iniCamera`

Alhora d'inicialitzar la càmera, també hem canviat una mica els paràmetres
del mètode per que rebin la mida de la finestra:

```c
void Escena::iniCamera(bool camGen, int a, int h){
   if(camGen == true){
       a = 600;//inicialmente 640x480 y
       h = 600; //al usarse Camera::AjustaAspectRatioWd no queda bien
       camGeneral.ini(a, h, capsaMinima);
       camGeneral.vs.obs = vec4(0.0, 20.0, 0.0, 1.0);
       camGeneral.piram.d = 2.0;
       camGeneral.piram.dant = 10.0;
       camGeneral.piram.dpost = 30.0;

       camGeneral.vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
                                  //ver Camera::CalculVup
       camGeneral.vs.angx = -90.0;//un giro de +90 apunta vup a z+
       camGeneral.vs.angy = 0.0;//un giro de +90 apunta el vetor forward de la camara a -x
       camGeneral.vs.angz = 0.0;//un giro de +90 apunta vup a x-
       vec3 vu = camGeneral.CalculVup(camGeneral.vs.angx, camGeneral.vs.angy, camGeneral.vs.angz);
       camGeneral.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);
       //camGeneral.vs.vup = vec4(0.0, 0.0, -1.0, 0.0);//con 0,1,0 la camara apunta a -z y no se ve

       camGeneral.CalculaMatriuModelView();
       camGeneral.CalculWindow(capsaMinima);
       camGeneral.CalculaMatriuProjection();
   }
}
```

De moment, treballarem amb la primera càmera, i quan s'introdueixi la càmera
en primera persona, s'introduirà els canvis per fer-la funcionar.

#### `Escena::setAnglesCamera`

Al canviar els angles de rotació del sistema de visualització, és necessari
recalcular el vector de verticalitat, `vup`, tornar a recalcular la posició de
l'observador, `obs`, i per tant, recalcular la matriu model-view:

```c
void Escena::setAnglesCamera(bool camGen, float angX, float angY, float angZ){
    if(camGen == true){
        camGeneral.vs.angx = angX;
        camGeneral.vs.angy = angY;
        camGeneral.vs.angz = angZ;
        vec3 vu = camGeneral.CalculVup(camGeneral.vs.angx, camGeneral.vs.angy, camGeneral.vs.angz);
        camGeneral.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);
        camGeneral.vs.obs = camGeneral.CalculObs(camGeneral.vs.vrp, camGeneral.piram.d, camGeneral.vs.angx, camGeneral.vs.angy);
        camGeneral.CalculaMatriuModelView();
    }
}
```

#### `Escena::setVRPCamera`

Si es canvia la posició del punt a on apunta el pla de projecció, s'haurà de
recalcular la matriu model-view:

```c
void Escena::setVRPCamera(bool camGen, point4 vrp){
    if(camGen == true){
        camGeneral.vs.vrp = vrp;
        camGeneral.CalculaMatriuModelView();
    }
}
```

#### `Escena::setWindowCamera`

És evident que si la finestra canvia, ja sigui per que es retalla, o per que
canvia, s'ha de recalcular la matriu de projecció:

```c
void Escena::setWindowCamera(bool camGen, bool retallat, Capsa2D window){
    if(camGen == true){
        camGeneral.wd = window;
        if(retallat == true){
            camGeneral.CalculWindowAmbRetallat();
        }
        camGeneral.AjustaAspectRatioWd();//amplia el window per tal que el seu aspect ratio sigui igual al del viewport
        camGeneral.CalculaMatriuProjection();
    }
}
```

#### `Escena::setDCamera`

Com que la distància entre els plans antero-posterior de la piràmide de
projecció paralela no canvien, no cal fer cap recalcul:

```c
void Escena::setDCamera(bool camGen, float d){
    if(camGen == true){
        camGeneral.piram.d = d;
    }
}
```

## 5 Modificació de la classe GLWidget per incloure la càmera general

### 5.1 Inicialitzar la càmera general

Hem introduït a la clase ``GLWidget` un nou atribut:

```c
bool cameraActual; //true para camara general
```

Aquest paràmetre ens permetrà saber amb quina càmera estem renderitzant. Quan
és `true`, s'està usant la càmera general, en cas contrari, s'estarà utilitzant
la càmera en primera persona.

Alhora de construir `GLWidget`:

```c
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    /* ... */

    cameraActual = true;
    esc = new Escena(this->size().width(), this->size().height());
    
    /* ... */
}
```

Com que ara la càmera serà la responsable de assegurar una projecció correcte,
s'ha esborrat la implementació d'`adaptaObjecteTamanyWidget`.

Dins les funcions esmentades a l'enunciat de la pràctica en aquest punt, s'han
fet els canvis necessaris per fer funcionar la càmera:

#### `GLWidget::initializeGL`

Li passem a `Escena` el shader program per gestionar de forma més còmode
l'enviament de les matrius model-view i de projecció:

```c
void GLWidget::initializeGL()
{
    //std::cout<<"GLWidget::initializeGL"<<std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShadersGPU();
    esc->pr = program;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(cameraActual==true){
        esc->camGeneral.toGPU(program);
    }
}
```

#### `GLWidget::paintGL`

```c
void GLWidget::paintGL()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   esc->actualizaMatr(cameraActual);
   esc->draw(cameraActual);

   /* ... */
}
```

#### `GLWidget::resizeGL`

```c
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    if(cameraActual==true){
        esc->camGeneral.setViewport(0, 0, width, height);
    }
}
```

### 5.2 Modificar interactivament la càmera general

Per poder ajustar els angles de projecció, en primer lloc, hem canviat el mètode
`qNormalizeAngle` per que acceptés angles com a flotants de doble precissió:

```c
static void qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
}
```

### 5.2.1 Arrosegant el ratólí amb el **botó esquerra** pitjat es canvien els **angles** de visió de la càmera.

Hem actualitzat els mètodes de rotacions per que poguesin canviar els angles de
rotació del sistema de visualització.

#### `GLWidget::setXRotation`

```c
void GLWidget::setXRotation(int angle)
{
    if (angle > 0) {
        esc->camGeneral.vs.angx += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angx -= 1.0;
    qNormalizeAngle(esc->camGeneral.vs.angx);
    esc->setAnglesCamera(cameraActual, esc->camGeneral.vs.angx, esc->camGeneral.vs.angy, esc->camGeneral.vs.angz);
    updateGL();
}
```

#### `GLWidget::setYRotation`

```c
void GLWidget::setYRotation(int angle)
{
    if (angle > 0) {
        esc->camGeneral.vs.angy += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angy -= 1.0;

    qNormalizeAngle(esc->camGeneral.vs.angy);
    esc->setAnglesCamera(cameraActual, esc->camGeneral.vs.angx, esc->camGeneral.vs.angy, esc->camGeneral.vs.angz);
    updateGL();

}
```

En comptes d'incrementar segons l'angle obtingut, ho fem amb el mateix increment
de desplaçament per veure més lentament el canvi.

Una vegada actualitzats els angles de desplaçament, fem un canvi sobre el mètode
`mouseMoveEvent` per actualitzar l'eix X o Y en funció del desplaçament del
ratolí:

```c
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(cameraActual == false)return;//solo funciona con camGeneral

    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();


    if (event->buttons() & Qt::LeftButton) {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
            //setYRotation(dx);
            //setXRotation(dy);
        }
        else if(lastPos.y()!= event->y()) {
            setXRotation(dy);
        }
        else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }
    }

    lastPos = event->pos();
}
```

### 5.2.2 Prement les tecles `+` o `-` es farà **zoom** de l'escena.

#### `GLWidget::Zoom`

Aquest mètode només ho fa per la càmera general:

```c
void GLWidget::Zoom (double inOut) {
    esc->camGeneral.AmpliaWindow(inOut);
    esc->camGeneral.CalculaMatriuProjection();
    updateGL();
}
```

Com que s'està modificant la window, és obvi que s'ha de calcular la matriu
de projecció abans d'entrar a renderitzar el frame.

#### `GLWidget::keyPressEvent`

Per gestionar el zoom amb les tecles demanades, s'han fet els següents canvis:

```c
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    /* ... */

    switch ( event->key() )
    {
        /* ... */
        case Qt::Key_Plus:
            Zoom(-0.05);
            break;
        case Qt::Key_Minus:
            Zoom(0.05);
            break;
    }

    /* ... */
}
```

### 5.2.3 Prement la tecla `Alt` simultàniament amb les tecles de les fletxes es farà un **panning 2D**

#### `GLWidget::Pan`

En primer lloc, s'ha implementat el mètode `Pan` per desplaçar la window
segons la direcció que s'hagi entrat per teclat:

```c
void GLWidget::Pan(double dx, double dy) {
    esc->camGeneral.wd.pmin.x = esc->camGeneral.wd.pmin.x + dx;
    esc->camGeneral.wd.pmin.y = esc->camGeneral.wd.pmin.y + dy;
    esc->camGeneral.CalculaMatriuProjection();
    updateGL();
}
```

Per modificar el centre de projecció, només cal moure el punt mínim del pla
anterior de la finestra per desplaçar la projecció. I òbviament, cal actualitzar
la matriu de projecció.

#### `GLWidget::keyPressEvent`

Des d'aquest mètode es crida el mètode `Pan` que hem creat:

```c
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    /* ... */

    switch ( event->key() )
    {
        /* ... */
        case Qt::Key_Up:
            if (event->modifiers() & Qt::AltModifier){
                Pan(0, 0.01);
                break;
            }
            /* ... */
            break;
        case Qt::Key_Down:
            if (event->modifiers() & Qt::AltModifier){
                Pan(0, -0.01);
                break;
            }
            /* ... */
            break;
        case Qt::Key_Left:
            if (event->modifiers() & Qt::AltModifier){
                Pan(-0.01, 0);
                break;
            }
            /* ... */
            break;
        case Qt::Key_Right:
            if (event->modifiers() & Qt::AltModifier){
                Pan(0.01, 0);
                break;
            }
            /* ... */
            break;
        /* ... */
    }

    /* ... */

}
```

## 6 Modificació de la classe Escena i GLWidget per incloure la càmera en primera persona

### 6.1 Modificació de la classe Escena per afegir la càmera en primera persona

Per implementar la càmera en primera persona s'ha introduit un nou atribut a la
clase `Escena`:

```c
// escena.h
Camera camFirstP;
```

D'aquesta, tindrem dues instàncies de la càmera, cadascuna amb les configuracions
de sistema de projecció i piràmide de visualització, i ens permetrà canviar d'una
càmera a l'altra de manera més senzilla i còmode.

S'han de modificar els següents mètodes de la càmera:

#### `Escena::iniCamera`

```c
void Escena::iniCamera(bool camGen, int a, int h){
    if(camGen == true){
        /* ... */
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
       camFirstP.piram.dant = 0.54;
       camFirstP.piram.dpost= 1.7;
       camFirstP.CalculaMatriuProjection();
   }
}
```

#### `Escena::setAnglesCamera`

Tot i que la implementació és la mateixa amb respecte la càmera general, només
canvia la instància de càmera que s'està usant, aquesta implementació ha anat
variant bastant, i per poder experimentar còmodament amb les diferents
projeccions, es deixa d'aquesta manera:

```c
void Escena::setAnglesCamera(bool camGen, float angX, float angY, float angZ){
    if(camGen == true){
        /* ... */
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
```

#### `Escena::setVRPCamera`

Al igual que l'anterior, l'únic que canvia és la instància de càmera usada

```c
void Escena::setVRPCamera(bool camGen, point4 vrp){
    if(camGen == true){
        /* ... */
    }else{
        camFirstP.vs.vrp = vrp;
        camFirstP.CalculaMatriuModelView();
    }
}
```

#### `Escena::setWindowCamera`

```c
void Escena::setWindowCamera(bool camGen, bool retallat, Capsa2D window){
    if(camGen == true){
        /* ... */
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
```

#### `Escena::setDCamera`

```c
void Escena::setDCamera(bool camGen, float d){
    if(camGen == true){
        camGeneral.piram.d = d;
    }else{
        camFirstP.piram.d = d;
        camFirstP.CalculaMatriuProjection();
    }
}
```

### 6.2 Modificació de la classe GLWidget per modificar interactivament la càmera en primera persona

Per introduir la càmera en primera persona i que funcionés s'han fet alguns
canvis aquí i allà

#### `GLWidget::initializeGL`

Al igual que amb la càmera general, s'envia les matrius a la GPU amb la nova
càmera implementada:

```c
void GLWidget::initializeGL()
{
    /* ... */

    if(cameraActual==true){
        esc->camGeneral.toGPU(program);
    }else{
        esc->camFirstP.toGPU(program);
    }
}
```

#### `GLWidget::resizeGL`

```c
void GLWidget::resizeGL(int width, int height)
{
    /* ... */

    if(cameraActual==true){
        esc->camGeneral.setViewport(0, 0, width, height);
    }else{
        esc->camFirstP.setViewport(0, 0, width, height);
    }
}
```

Després, alhora d'afegir nous objectes a l'escena, s'han fet alguns canvis a la
instanciació dels objectes:

#### `GLWidget::newObjecte`

Quan s'instància un nou objecte, recalculem la capsa mínima contenidora de
l'escena i actualitzem el vector `vrp` amb el nou objecte instanciat.

```c
void GLWidget::newObjecte(Objecte * obj)
{
    point4 vrp;

    esc->addObjecte(obj);
    esc->CapsaMinCont3DEscena();
    vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
    vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
    vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);
    vrp[3] = 1.0;
    esc->setVRPCamera(cameraActual, vrp);
    if(cameraActual == true){
        esc->camGeneral.CalculWindow(esc->capsaMinima);
        esc->camGeneral.AmpliaWindow(-0.1);
    }else{
        //por si apretamos B antes de cargar obj
        esc->camFirstP.vs.vrp = point4(0.0, 0.0307474, 0.5, 1.0);
    }


    updateGL();
}
```

En funció de la càmera usada, hem tingut que fer uns petits ajustos puntuals
per que tot quedés correctament definit.

El mateix ha passat alhora d'afegir el conjunt de boles o la sala de billar:

#### `GLWidget::newConjuntBoles`

```c
void GLWidget::newConjuntBoles()
{
    point4 vrp;
    ConjuntBoles *conjuntboles = new ConjuntBoles();

    esc->conjuntBoles = conjuntboles;
    for(int i=0; i<conjuntboles->listaConjuntBoles.size(); i++){
        esc->listaObjectes.push_back(conjuntboles->listaConjuntBoles[i]);
        listaCapsasConjuntBoles.push_back(conjuntboles->listaConjuntBoles[i]->calculCapsa3D());
    }

    esc->CapsaMinCont3DEscena();
    vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
    vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
    vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);
    vrp[3] = 1.0;
    esc->setVRPCamera(cameraActual, vrp);
    if(cameraActual == true){
        esc->camGeneral.CalculWindow(esc->capsaMinima);
        esc->camGeneral.AmpliaWindow(-0.1);
    }else{
        esc->camFirstP.vs.vrp = point4(0.0, 0.0307474, 0.5, 1.0);
    }

    updateGL();
}
```

#### `GLWidget::newSalaBillar`

```c
void GLWidget::newSalaBillar()
{
    PlaBase *plaBase = this->newPlaBs();
    esc->addObjecte(plaBase);
    cT = plaBase->calculCapsa3D();//para el calculo de colisiones

    Bola *bolab = new Bola(0.0, 0.03075, 0.5, 0.03075, 1.0, 1.0, 1.0, "0");//x0,y0,z0,r,R,G,B,numBola
    esc->addObjecte(bolab);
    cb = bolab->calculCapsa3D();//para el calculo de colisiones
    ctrB = vec3(0.0, 0.03075, 0.5);

    ConjuntBoles *conjuntboles = new ConjuntBoles();
    esc->conjuntBoles = conjuntboles;
    for(int i=0; i<conjuntboles->listaConjuntBoles.size(); i++){
        esc->listaObjectes.push_back(conjuntboles->listaConjuntBoles[i]);
        listaCapsasConjuntBoles.push_back(conjuntboles->listaConjuntBoles[i]->calculCapsa3D());
    }

    esc->CapsaMinCont3DEscena();
    point4 vrp;
    vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
    vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
    vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);
    vrp[3] = 1.0;

    esc->camGeneral.vs.vrp = point4(vrp[0], vrp[1] , vrp[2], 1.0);

    esc->camGeneral.CalculWindow(esc->capsaMinima);
    esc->camGeneral.AmpliaWindow(-0.1);

    esc->camFirstP.vs.vrp = point4(0.0, 0.0307474, 0.5, 1.0);

    updateGL();
}
```

#### `GLWidget::keyPressEvent`

Finalment, per seleccionar quina càmera es vol usar, s'ha modificat el mètode
`keyPressEvent`:

```c
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    /* ... */

    switch ( event->key() )
    {
        /* ... */
        
        case Qt::Key_B:
            cameraActual = false;//camFirstP
            break;
        case Qt::Key_T:
            cameraActual = true;//camGen
            break;
        
        /* ... */
    }

    /* ... */

}
```

Com que a `PaintGL` ja se li passa a l'`Escena` quin tipus de càmera es vol
utilitzar, no cal fer res més.

### 6.3 Modificació de la càmera en primera persona amb el moviment de la bola blanca

Per a aquest últim pas de la pràctica s'han introduit algunes noves variables,
arrel del refactor del càlcul de col·lisions inicial:

```c
// glwidget.h
vec3 ctrB;
```

Quan es fa el càlcul de col·lisions, utilitzem aquesta variable per guardar la
posicíó del centre de la bola.

#### `GLWidget::keyPressEvent`

El codi introduit, a més de recalcular la posició del centre de la bola amb la
nova variable creada, i calcular la matriu de translació per moure la bola 
blanca, s'ha afegit una nova condició per verificar uns calculs sobre el `vrp`
que només es faran en la càmera en primera persona.

```c
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (esc->bolaBlanca!=NULL && esc->plaBase!=NULL && esc->conjuntBoles!=NULL){
        esc->computeCollisions(cb, cT, ctrB, listaCapsasConjuntBoles, event);
    }

    switch ( event->key() )
    {
        /* ... */
        case Qt::Key_Up:
            /* ... */
            m = Translate(0.0,  0.0, esc->dzN);
            cb.pmin.z += esc->dzN;
            ctrB.z += esc->dzN;
            break;
        case Qt::Key_Down:
            /* ... */
            m = Translate(0.0,  0.0, esc->dzP);
            cb.pmin.z += esc->dzP;
            ctrB.z += esc->dzP;
            break;
        case Qt::Key_Left:
            /* ... */
            m = Translate(esc->dxN,  0.0, 0.0);
            cb.pmin.x += esc->dxN;
            ctrB.x += esc->dxN;
            break;
        case Qt::Key_Right:
            /* ... */
            m = Translate(esc->dxP,  0.0, 0.0);
            cb.pmin.x += esc->dxP;
            ctrB.x += esc->dxP;
            break;
        /* ... */
    }

    if (esc->bolaBlanca!=NULL){
        esc->bolaBlanca->aplicaTG(m);

        if(cameraActual == false){
            esc->setVRPCamera(false, point4(ctrB.x, ctrB.y, ctrB.z, 1.0));
            esc->camFirstP.vs.obs = vec4(ctrB.x, 0.0307474, cb.pmin.z + (cb.p/2.0) + 0.1, 1.0);
            esc->camFirstP.piram.dpost = esc->camFirstP.vs.obs.z - esc->capsaMinima.pmin.z + 0.1;
        }
   }
}
```

Ajustem l'observador per que miri a la bola blanca, i ajustem el pla posterior
de la piràmide de projecció per que es puguin veure totes les boles.
