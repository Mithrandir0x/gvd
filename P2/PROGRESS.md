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

- A `Escena::~Escena` Evitar destruir objectes nuls.

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

**WARNING**
> Verificar matrius amb identitat per veure si s'estan passant correctament les
> matrius model-view i projection.

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

### Comprobació dels mètodes

Per veure si estem passant les matrius correctament, 

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

> *Annotacions*
> - És el VRP el COP?

> **WARNING**
>
> - Verificar que els arguments de totes dues funcions s'utilitzen correctament.

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

> **WARNING**
> - La capsa mínima que rep per paràmetre, de qui és? És la capsa mínima de
> l'escena o es la capsa de l'objecte al que apunta la càmera?
> - Preguntar qué fa la funció `Camera::VertexCapsa3D`


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

> Annotacions
>
> - Tècnicament, s'hauria de calcular l'amplada i l'alçada del viewport, i el VRP.
>   Però aixó entra una mica en conflicte amb el punt anterior, on es demana que
>   s'estigui mirant al punt `(0, 0, 0)`. (Crec...)
>   - L'altra possibilitat és que a partir del càlcul de la window, que encara no
>     està implementat, es pot calcular els punts 

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
   }else{
       //inicializar camera en primera persona
   }

}
```

De moment, treballarem amb la primera càmera, i quan s'introdueixi la càmera
en primera persona, s'introduirà els canvis per fer-la funcionar.

#### `Escena::setAnglesCamera`

```c
void Escena::setAnglesCamera(bool camGen, float angX, float angY, float angZ){
    if(camGen == true){
        camGeneral.vs.angx = angX;
        camGeneral.vs.angy = angY;
        camGeneral.vs.angz = angZ;
        vec3 vu = camGeneral.CalculVup(camGeneral.vs.angx, camGeneral.vs.angy, camGeneral.vs.angz);
        camGeneral.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);
        camGeneral.CalculaMatriuModelView();
        CapsaMinCont3DEscena();
        camGeneral.CalculWindow(capsaMinima);
        camGeneral.CalculaMatriuProjection();

    }
}
```

#### `Escena::setVRPCamera`

```c
void Escena::setVRPCamera(bool camGen, point4 vrp){
    if(camGen == true){
        camGeneral.vs.vrp = vrp;
        camGeneral.CalculaMatriuModelView();
        CapsaMinCont3DEscena();
        camGeneral.CalculWindow(capsaMinima);
        camGeneral.CalculaMatriuProjection();
    }
}
```

#### `Escena::setWindowCamera`

```c
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
```

#### `Escena::setDCamera`

```c
void Escena::setDCamera(bool camGen, float d){
    if(camGen == true){
        camGeneral.piram.d = d;
        CapsaMinCont3DEscena();
        camGeneral.CalculWindow(capsaMinima);
        camGeneral.CalculaMatriuProjection();
    }
}
```

## 5 Modificació de la classe GLWidget per incloure la càmera general

### 5.1 Inicialitzar la càmera general

> NO IMPLEMENTAT

### 5.2 Modificar interactivament la càmera general

> NO IMPLEMENTAT

## 6 Modificació de la classe Escena i GLWidget per incloure la càmera en primera persona

### 6.1 Modificació de la classe Escena per afegir la càmera en primera persona

S'han de modificar els següents mètodes de la càmera:

#### `Escena::iniCamera`

> NO IMPLEMENTAT

#### `Escena::setAnglesCamera`

> NO IMPLEMENTAT

#### `Escena::setVRPCamera`

> NO IMPLEMENTAT

#### `Escena::setWindowCamera`

> NO IMPLEMENTAT

#### `Escena::setDCamera`

> NO IMPLEMENTAT

### 6.2 Modificació de la classe GLWidget per modificar interactivament la càmera en primera persona

> NO IMPLEMENTAT

### 6.3 Modificació de la càmera en primera persona amb el moviment de la bola blanca

> NO IMPLEMENTAT
