
# Progrés de la pràctica

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
model_view = program->uniformLocation("model_view");
glUniformMatrix4fv( model_view, 1, GL_TRUE, m );
```

#### `Camera::setProjectionToGPU`

Es demana un registre uniforme per passar la matriu Projection:

```c
projection = program->uniformLocation("projection");
glUniformMatrix4fv( projection, 1, GL_TRUE, p );
```

#### `Camera::toGPU`

Executem els dos mètodes implementats:

```c
setModelViewToGPU(program, modView);
setProjectionToGPU(program, projection);

program->link();
program->bind();
```

### 3.2 Implementació de diferents utilitats de la classe Camera

S'han d'implementar els mètodes indicats:

#### `Camera::ini`

En primer lloc, es calcula el View Reference Point a partir de la capsa
contenidora passada per paràmetre:

```c
vs.vrp[0] = capsaMinima.pmin[0] + ( capsaMinima.a / 2. );
vs.vrp[1] = capsaMinima.pmin[1] + ( capsaMinima.h / 2. );
vs.vrp[2] = capsaMinima.pmin[2] + ( capsaMinima.p / 2. );

setViewport(0, 0, a, h);
```

#### `Camera::CalculaMatriuModelView`

A partir de com està explicat a la diapositiva 22 de la segona sessió del
tema 3, fem les següents transformacions geomètriques:

```c
modView = LookAt(vs.obs, vs.vrp, vs.vup);
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
GLfloat left, right, bottom, top, zNear, zFar;

left = wd.pmin.x;
right = wd.pmin.x + wd.a;
bottom = wd.pmin.y;
top = wd.pmin.y + wd.h;
zNear = piram.dant;
zFar = piram.dpost;

// CODI A MODIFICAR DURANT LA PRACTICA 2
if ( piram.proj == PARALLELA ) {
    proj = Ortho(left, right, bottom, top, zNear, zFar);
} else if ( piram.proj == PERSPECTIVA ) {
    proj = Frustum(left, right, bottom, top, zNear, zFar);
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
vec4  vaux[8];
mat4  MDP;
int i;

// Calcular la modelview o reutilitzar l'existent?
//modView = LookAt(vs.obs, vs.vrp, vup);

// Es deforma la matriu per que estigui en perspectiva?
if ( piram.proj == PERSPECTIVA ) {
    CreaMatDp(MDP);
    modView = MDP * modView;
}

// Transforma la capsa 3D en un vector amb cadascun dels
// vèrtex de la capsa mínima
VertexCapsa3D(c, vaux);

// Es transforma en coordenades de càmera els punts anteriors
for( i = 0 ; i < 8 ; i++ ) {
    vaux[i] = modView * vaux[i];
}
// i es calcula la capsa mínima 2D a partir de les coordenades
wd = CapsaMinCont2DXYVert(vaux, 8);
```

## 4 Modificació de la classe Escena per incloure la càmera general

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

A la implementació del constructor d'Escena, `Escena::Escena`, s'ha afegit
la inicialització de l'objecte `camGeneral`:

```c
// Inicialització de la càmera
camGeneral = new Camera();
```

Per a situar la càmera al punt `(0, 20, 0)` i estigui mirant al punt `(0, 0, 0)`,
s'ha definit aquesta funció:

```c
void Escena::SetZenitCamera()
{
    // Situem "l'objectiu" que mira la càmera a (0, 0, 0), a una distància de 20
    // respecte l'eix de la Y.
    camGeneral.vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
    camGeneral.vs.vup = vec4(0.0, 0.0, 1.0, 1.0);
    camGeneral.vs.obs = camGeneral->CalculObs(camGeneral->vs.vrp, 20, 0, 90.0);
}
```

### 4.2 Implementació de diferents utilitats referents a la càmera la classe Escena

#### `Escena::iniCamera`

> Annotacions
>
> - Tècnicament, s'hauria de calcular l'amplada i l'alçada del viewport, i el VRP.
>   Però aixó entra una mica en conflicte amb el punt anterior, on es demana que
>   s'estigui mirant al punt `(0, 0, 0)`. (Crec...)
>   - L'altra possibilitat és que a partir del càlcul de la window, que encara no
>     està implementat, es pot calcular els punts 

```c
void Escena::iniCamera(bool isCamGeneral) {
    if ( isCamGeneral ) {
        Capsa3D capsaMinima = CapsaMinCont3DEscena();
        camGeneral.ini(screenSize[0], screenSize[1], capsaMinima);
        SetZenitCamera();
    } else {
    }
}
```

#### `Escena::setAnglesCamera`

> NO IMPLEMENTAT

#### `Escena::setVRPCamera`

> NO IMPLEMENTAT

#### `Escena::setWindowCamera`

> NO IMPLEMENTAT

#### `Escena::setDCamera`

> NO IMPLEMENTAT

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

---

# Correccions sobre la pràctica 1

> **WARNING**
>
> **PODEN HAVER MÉS CORRECCIONS, DEGUT A QUE NO S'HA POGUT EXECUTAR LA PRÀCTICA**

## `PlaBase`

- Canviar la manera en que es defineixen les coordenades inicials del pla base. En comptes que estiguin en el constructor, haurien d'estar definides a través de métodes GET/SET i cridades des d'`Escena`.

> S'ha comentat tot el codi pertinent a l'escalat s'ha traslladat al mètode `GLWidget::adaptaObjecteTamanyWidget`. Aquest canvis
> estan esmentats més a sota.
> 
> Però també s'ha canviat la creació del `PlaBase`:
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
}
> ```

## `Bola`

- `Bola::triangle` -> tenemos codigo repetido, don't noooooo

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

- Descomentar `GLWidget::adaptaObjecteTamanyWidget`, i verificar que al treure els escalats dels constructors funciona correctament l'adaptació a la capsa (-1,1).

> S'ha implementat el mètode `GLWidget::adaptaObjecteTamanyWidget`:
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
>             double aristaMax = 0.0;
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
>             //capsa = obj->calculCapsa3D();
> }
> ```

## `Escena`

- A `Escena::aplicaTGCentrat`, s'ha d'aplicar la transformació de la matriu respecte el centre de la capsa contenidora de l'escena.

> S'ha reimplementat el mètode per fer el que toca:
>
> ```c
> void Escena::aplicaTGCentrat(mat4 m) {
> 
>     // Metode a modificar
> 
>     if (taulaBillar!=NULL)
>         taulaBillar->aplicaTGCentrat(m);
>     if (plaBase!=NULL)
>         plaBase->aplicaTGCentrat(m);
>     if (bolaBlanca!=NULL)
>         bolaBlanca->aplicaTGCentrat(m);
>     if (conjuntBoles!=NULL){
>             for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
>                     conjuntBoles->listaConjuntBoles[i]->aplicaTGCentrat(m);
>             };
>     }
> 
> }
> ```

- A `Escena::~Escena` Evitar un "NULLPOINTEREXCEPTION" al destruir els objectes, comprovant abans que existeixin (i.e. no siguin nuls)

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

