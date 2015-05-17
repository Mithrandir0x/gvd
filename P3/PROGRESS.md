# Abstract

Per seguir aquest informe de pràctica s'ha de tenir en compte que s'ha partit
des de la implementació existent de la segona pràctica.

---

# Correccions sobre la pràctica 2

Aquesta primera part es dedicarà a descriure els canvis introduïts fets arrel
de les observacions donades per la segona pràctica.

- Hem interpretat que amb la càmera en primera persona és de la bola, i que al
  premer les tecles per moure cap a l'esquerra o la dreta feia que moves la bola
  i no l'angle de la càmera com si fos el pal. El dia del test, la profesora ens
  va comentar que s'havia de canviar aquestes coses i només vam poguer arreglar
  el fet que no es veia la bola (el canvi és que ara és la càmera en primera
  persona del pal, en comptes de ser la de la bola).

- La implementació presentada de la segona extensió s'ha desactivat, degut a
  possibles problemes de rendiment quan es transicionava de càmera general a
  càmera en primera persona. No obstant, el codi està implementat a `GLWidget::keyPressEvent`.

---

# Progrés de la pràctica

## 1. Creació d'una nova classe `Llum`

Inicialment, hem fet que la classe `Llum` es pogués referir a diferents instàncies
de llums al vertex o al fragment shader. A més, compta amb un "flag" booleà per
indicar si està encessa aquesta llum o no.

```c
// llum.h

class Llum
{
public:
    Llum();
    Llum(vec4 pos, vec4 dir, vec4 iA, vec4 iD, vec4 iS, float bAngle, float atConstant, float atLin, float atQuad, bool on);
    ~Llum();
    void toGPU(QGLShaderProgram *program, const char* struc);

    vec4 lightPosition;
    vec4 lightDirection;
    vec4 Ia, Id, Is;
    float bmAngle, at_constant, at_linear, at_quadratic;
    bool ON;

    struct{
        GLuint position;
        GLuint direction;
        GLuint iA;
        GLuint iD;
        GLuint iS;
        GLuint beamAngle;
        GLuint att_constant, att_linear, att_quadratic;
    }gl_IdLlum;
};
```

Un petit detall esmentat abans sobre la llum:

```c
void Llum::toGPU(QGLShaderProgram *program, const char* struc)
{

    gl_IdLlum.position = program->uniformLocation((const char*)(struc + string(".gpuLightPosition")).c_str());
    gl_IdLlum.direction = program->uniformLocation((const char*)(struc + string(".gpuLightDirection")).c_str());
    gl_IdLlum.iA = program->uniformLocation((const char*)(struc + string(".gpuIa")).c_str());
    gl_IdLlum.iD = program->uniformLocation((const char*)(struc + string(".gpuId")).c_str());
    gl_IdLlum.iS = program->uniformLocation((const char*)(struc + string(".gpuIs")).c_str());
    gl_IdLlum.beamAngle = program->uniformLocation((const char*)(struc + string(".gpuBeamAngle")).c_str());
    gl_IdLlum.att_constant= program->uniformLocation((const char*)(struc + string(".gpuAtt_constant")).c_str());
    gl_IdLlum.att_linear = program->uniformLocation((const char*)(struc + string(".gpuAtt_linear")).c_str());
    gl_IdLlum.att_quadratic = program->uniformLocation((const char*)(struc + string(".gpuAtt_quadratic")).c_str());

    glUniform4fv(gl_IdLlum.position, 1, lightPosition);
    glUniform4fv(gl_IdLlum.direction, 1, lightDirection);
    glUniform4fv(gl_IdLlum.iA, 1, Ia);
    glUniform4fv(gl_IdLlum.iD, 1, Id);
    glUniform4fv(gl_IdLlum.iS, 1, Is);
    glUniform1f(gl_IdLlum.beamAngle, bmAngle);
    glUniform1f(gl_IdLlum.att_constant, at_constant);
    glUniform1f(gl_IdLlum.att_linear, at_linear);
    glUniform1f(gl_IdLlum.att_quadratic, at_quadratic);
}
```

Hem pensat que la manera més senzilla per passar les llums seria tenir unes
variables uniformes ja definides, i només pactem el nom que s'usarà al enviar
la informació de la llum als shaders.

L'argument `struc` indica a quina de les variables del shader s'envia els
paràmetres de la llum.

### 1.1 La classe `ConjuntLlums`

Per a fer més còmode la instanciació de múltiples llums a l'escena, s'ha
implementat aquesta classe per a gestionar ahí totes les llums que hi hauràn.

```c
// conjuntllums.h
#ifndef CONJUNTLLUMS_H
#define CONJUNTLLUMS_H

#include <Common.h>
#include <llum.h>

class QGLShaderProgram;

class ConjuntLlums
{
public:
    ConjuntLlums();
    ~ConjuntLlums();
    void toGPU(QGLShaderProgram *program);

    Llum *luz1, *luz2, *luz3;
};

#endif // CONJUNTLLUMS_H

```

Es podria utilitzar estructures més complexes per a guardar les llums, però
donat que ens interesa més experimentar amb els diferents atributs de les llums
creiem que aquesta manera és més còmode per gestionar puntualment les llums.

### 1.2 Introducció de les llums a l'`Escena`

A la capçalera de la classe `Escena` hem introduit dues variables noves,
`AmbientLight`, que tindrà el valor de llum ambiental de l'escena i la variable
`conjllums`, que contindrà totes les llums de l'escena.

A més, per poder enviar la nova informació que estem afegint a la GPU, s'han
introduït dos mètodes nous:

```c
// escena.h

// ...

#include <conjuntllums.h>

// ...

class Escena:
{
    // ...
    void setAmbientToGPU(QGLShaderProgram *program);
    void setconTexturaToGPU(QGLShaderProgram *program, bool conText);

    // ...
    vec4 AmbientLight;
    ConjuntLlums *conjllums;
    bool conTextura = true;
}
```

Com es pot veure, a més d'incloure les llums, hem inclós la llum ambiental que
tindrà l'escena.

#### `Escena::Escena`

En el constructor d'`Escena`, hem inicialitzat aquestes variables:

```c
Escena::Escena(int vpa, int vph)
{
    // ...
    
    AmbientLight = vec4(0.4, 0.4, 0.4, 1.0);
    conjllums = new ConjuntLlums();
}
```

#### `Escena::setAmbientToGPU`

Aquest mètode s'encarregarà d'enviar els valors de llum ambiental a la GPU:

```c
void Escena::setAmbientToGPU(QGLShaderProgram *program){
    GLuint LuzAmbLocation = program->uniformLocation("LuzAmbiente");
    glUniform4fv(LuzAmbLocation, 1, AmbientLight);
}
```

#### `Escena::setconTexturaToGPU`

Aquest mètode s'ha implementat per poder indicar els shaders si han de mostrar
la textura o no.

```c
void Escena::setconTexturaToGPU(QGLShaderProgram *program, bool conText)
{
    GLuint conTextLocation = program->uniformLocation("conTextura");
    glUniform1f( conTextLocation, conText );
}
```

#### `Escena::draw`

Finalment, per enviar tota la informació cap a la GPU, s'ha modificat el mètode
`draw` d'`Escena`:

```c
void Escena::draw(bool cameraActual) {
    if (taulaBillar!=NULL){
        // ...
        setAmbientToGPU(pr);
        conjllums->toGPU(pr);
        setconTexturaToGPU(pr, conTextura);
        // ...
    }

    if (plaBase!=NULL){
        // ...
        setAmbientToGPU(pr);
        conjllums->toGPU(pr);
        setconTexturaToGPU(pr, conTextura);
        // ...
    }

    if (bolaBlanca!=NULL){
        // ...
        setAmbientToGPU(pr);
        conjllums->toGPU(pr);
        setconTexturaToGPU(pr, conTextura);
        // ...
    }

    if (conjuntBoles!=NULL){
        for (int i=0; i<conjuntBoles->listaConjuntBoles.size(); i++) {
            // ...
            setAmbientToGPU(pr);
            conjllums->toGPU(pr);
            setconTexturaToGPU(pr, conTextura);
            // ...
        }
    }

}
```

## 2. Creació d'una nova classe `Material`

Aquesta classe és bastant directe, i no hi ha molt a comentar amb la pròpia classe en si.

```c
class Material
{
public:
    Material();
    Material(vec4 cA, vec4 cD, vec4 cE, float coefRefEsp);
    ~Material();
    void toGPU(QGLShaderProgram *program);

    //atributos opticos del material
    vec4 cA, cD, cE;
    float coefRefEsp;

    struct
    {
        GLuint cA;
        GLuint cD;
        GLuint cE;
        GLuint coefRefEsp;
    }gl_IdMaterial;
};
```

### 2.1 Introducció del material a la classe `Objecte`

Però, donat que qualsevol objecte serà d'un material, sí cal destacar els canvis que s'han
introduït a la classe `Objecte`.

S'ha introduit una nova variable `mat`, que guardarà el material de l'objecte:

```c
// objecte.h

class Objecte : public QObject
{
protected:
    // ...
    Material *mat;
    // ...
}
```

#### `Objecte::toGPU`

Per a enviar el material a la GPU, s'ha modificat el mètode `toGPU` d'`Objecte`
per a enviar-lo:

```c
void Objecte::toGPU(QGLShaderProgram *pr){
    // ...
    this->mat->toGPU(program);
}
```

### 2.2 Definició dels materials dels objectes de l'escena

Com que a `GLWidget` estem afegint els diferents objectes que hi hauran a
l'escena, hem fet modificacions sobre tots aquells mètodes que introduiexin
objectes a l'escena per incloure el material del que serà l'objecte.

A més, hem canviat la definició dels constructors de `PlaBase` i `Bola` per a
que incloguin una variable de tipus `Material` com a paràmetre.

#### `GLWidget::newPlaBs`

```c
PlaBase* GLWidget::newPlaBs(){
    // ...

    vec3 ka = vec3(0.19125, 0.0735, 0.0225);
    vec3 kd = vec3(0.7038, 0.27048, 0.0828);
    vec3 ke = vec3(0.256777, 0.137622, 0.086014);
    float kre = 0.1*128;

    Material *mat = new Material(ka, kd, ke, kre);

    PlaBase *plaBase = new PlaBase(v0, v1, v2, v3, cv0, cv1, cv2, cv3, mat);
    return plaBase;
}
```

#### `GLWidget::newBola`

```c
void GLWidget::newBola()
{
    vec4 ka = vec4(0.25, 0.20725, 0.20725, 1);
    vec4 kd = vec4(1.0, 0.829, 0.829, 1);
    vec4 ke = vec4(0.296648, 0.296648, 0.296648, 1);
    float kre = 0.088*128;

    Material *mat = new Material(ka, kd, ke, kre);
    Bola *bolablanca = new Bola(0.0, 0.03075, 0.5, 0.03075, 1.0, 1.0, 1.0, mat, tipoShading, "0");//x0,y0,z0,r,R,G,B,material,numBola
    newObjecte(bolablanca);
}
```

#### `GLWidget::newSalaBillar`

```c
void GLWidget::newSalaBillar()
{
    PlaBase *plaBase = this->newPlaBs();
    esc->addObjecte(plaBase);
    cT = plaBase->calculCapsa3D();//para el calculo de colisiones

    /* QString fileName = "/home/jj/Qtexamples/P1/resources/taula.obj";
    if (!fileName.isNull())
        newObj(fileName);
    cT = esc->taulaBillar->calculCapsa3D();*/
    vec4 ka = vec4(0.25, 0.20725, 0.20725, 1);
    vec4 kd = vec4(1.0, 0.829, 0.829, 1);
    vec4 ke = vec4(0.296648, 0.296648, 0.296648, 1);
    float kre = 0.088*128;

    Material *mat = new Material(ka, kd, ke, kre);
    Bola *bolab = new Bola(0.0, 0.03075, 0.5, 0.03075, 1.0, 1.0, 1.0, mat, tipoShading, "0");//x0,y0,z0,r,R,G,B,material,numBola
    esc->addObjecte(bolab);

    ConjuntBoles *conjuntboles = new ConjuntBoles(tipoShading);
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

## 3. Implementació dels diferents tipus de shading

Abans d'entrar amb les diferents implementacions per cadascun dels tipus de shadings
demanats a l'enunciat cal esmentar els canvis que s'han afet arràn del codi per
poder fer-los funcionar correctament.

### 3.1 Inclusió de les normals del pla dels triangles d'un `Objecte`

Com que utilitzarem el model d'iluminació Blinn-Phong, serà necessari passar les
normals de cadascun dels plans a calcular el color del vèrtex en funció de les
llums passades al shader.

En primer lloc, s'ha inclós el nou tipus `normal3`, un vector de tres components.
Ara, en comptes d'estar enviant cap a la GPU el color de cada vèrtex, s'enviarà
la normal del pla del qual forma part aquell vèrtex.

```c
// objecte.h

// ...
typedef Common::vec3  normal3;
// ...

class Objecte : public QObject
{
//...
public:
//...
    normal3 *normal;
//...
}

```

Desprès, s'ha afegit com a membre de la clase `Objecte` el vector `normal`, a
on es guardaran les normals de cadascun dels plans que formen l'objecte.

#### `Objecte::Objecte`

El constructor ha canviat per a adaptar-se al nou tipus de dada.

```c
Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints), QObject(parent)
{
    // ...
    normal = new normal3[npoints];
}
```

#### `Objecte::toGPU`

Alhora d'enviar l'objecte a la GPU, s'ha modificat aquest mètode per tenir en
compte la nova grandària del tipus de dada enviat:

```c
void Objecte::toGPU(QGLShaderProgram *pr){

    // ...
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * Index, sizeof(vec3) * Index, &normal[0] );
    // ...
    int normalLocation = program->attributeLocation("vNormal");
    // ...
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(vec4)*Index, 3);
    // ...
    program->bindAttributeLocation("vNormal", normalLocation);
}
```

### 3.2 Càlcul de les normals de `PlaBase`

Per a cadascún dels triangles que formen el pla base, s'ha calculat el producte
vectorial dels vectors dels plans per tenir el vector perpendicular, la normal,
del pla format.

#### `PlaBase::quad`

Abans de calcular els vèrtex de cada punt a enviar a la GPU, es calcula la
normal, i s'assigna a cadascun d'aquests punts.

```c
void PlaBase::quad( int a, int b, int c, int d )
{
    normal3 n = cross(vertices[a] - vertices[b],vertices[b] - vertices[c]);

    /* ... */ normal[Index] = n; /* ... */
    /* ... */ normal[Index] = n; /* ... */
    /* ... */ normal[Index] = n; /* ... */

    n = cross(vertices[a] - vertices[c],vertices[c] - vertices[d]);

    /* ... */ normal[Index] = n; /* ... */
    /* ... */ normal[Index] = n; /* ... */
    /* ... */ normal[Index] = n; /* ... */
}
```

Independentment del tipus de shading que s'estigui utilitzant, les normals es
calculen de la mateixa manera, a diferència de la classe `Bola`, com veurem a
continuació.

### 3.3 Càlcul de les normals de `Bola`

A diferència del pla base, la bola requereix una mica més de complexitat per a
suportar il·luminació amb els diferents shaders.

Donat que entre el shading `Flat` i `Gouraud` canvia la normal que s'ha d'usar,
s'ha introduït un nou paràmetre per a indicar en quin tipus de shading està per
a saber quina normal ha de calcular.

```c
// bola.h
class Bola : public Objecte
{
private:
    // ...
    QString tipoShading;
};
```

#### `Bola::Bola`

Amb la introducció del nou atribut, s'ha canviat la firma del constructor per
poder indicar el tipus de shading desitjat.

#### `Bola::triangle`

Quan es generen els triangles de la bola, al igual que amb el pla base, es
calcula la normal del pla definit pels vèrtex passats per paràmetre, però aquí
és on, depenent dels shading usat, assignem la normal (en el cas que sigui
`Flat`), o assignem les components pròpies del vèrtex en cadascuna de les
iteracions de la generació del pla (en el cas que sigui `Gouraud`).

```c
void Bola::triangle(const vec4& a, const vec4& b, const vec4& c )
{
    double u, v;

    vec4 vertArr[3] = {a, b, c};
    normal3 n = cross(vertArr[0] - vertArr[1],vertArr[1] - vertArr[2]); //normal para flat shading

    for(int i =0; i<3;i++){
        points[Index] = vertArr[i];
        //colors[Index] = this->color;//para el shading en lugar del color de cada vertex usamos su normal
        if(this->tipoShading == "Flat"){
            normal[Index] = n;
        }else{
            normal[Index] = vec3(vertArr[i].x, vertArr[i].y, vertArr[i].z);
        }

        u = 0.5 + atan2(-points[Index].z, -points[Index].x)/(2*M_PI);
        v = 0.5 - asin(-points[Index].y)/M_PI;
        if(u < 0.0)u=0.0;
        if(v < 0.0)v=0.0;
        if(u > 1.0)u=1.0;
        if(v > 1.0)v=1.0;
        vertexsTextura[Index] = vec2(u , v);
        Index++;
    }
}
```

Donat que en el nostre cas, al generar els plans de l'esfera els propis vèrtex
del pla calculat ja són normals, en el cas de calcular les normal pel `Gouraud`
només cal assignar el propi vèrtex del punt del pla.

En altres casos, hauriem de calcular la normal del pla realment.

### 3.4 Canvi de tipus de shading des de `GLWidget`

Finalment, ja tenim tots els canvis necessaris a punt, i només ens queda fer
que en `GLWidget` s'encarregui de canviar dins al programa shader els tipus de
shadings que s'implementaran.

S'han introduit una sèrie de fitxers codi `GLSL` per a cadascun dels tipus de 
shading:

- Per a `Flat` i `Gouraud`, s'ha creat els fitxers `vshaderFlatGouraud.glsl` i
  `fshaderFlatGouraud.glsl`, pels vèrtes i fragment shaders respectivament.
- Per a `Phong` s'ha creat els fitxers `vshaderPhong.glsl` i `fshaderPhong.glsl`.
- Per a `Toon`, s'ha creat els fitxers `vshaderToon.glsl` i `fshaderToon.glsl`.

Abans de començar a implementar-los, hem introduït uns petits canvis a la clase
`GLWidget` per afegir nous atributs i mètodes.

```c
// glwidget.h
class GLWidget : public QGLWidget
{
    // ...
public slots:
    // ...
protected:
    // ...
    void reinit(QString Shading, bool Text);
private:
    // ...
    QGLShader *vshaderFG;
    QGLShader *fshaderFG;
    QGLShader *vshaderP;
    QGLShader *fshaderP;
    QGLShader *vshaderT;
    QGLShader *fshaderT;
    // ...
    QString tipoShading;
    // ...
    void changeShadersGPU();
}
```

#### `GLWidget::GLWidget`

En el constructor, a més afegit la inicialització d'alguns paràmetres per poder
seleccionar quin tipus de shading es vol començar a usar.

```c
GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    // ...
    tipoShading = "Flat";
    esc->conTextura = false;
    // ...
}
```

#### `GLWidget::InitShader`

Ara, en comptes d'inicialitzar un shader, iniciarem tots al mateix temps,
compilant-los, i connectant al program shader el que s'hagi indicat al
constructor de la classe.

```c
void GLWidget::InitShader()
{
    vshaderFG = new QGLShader(QGLShader::Vertex, this);
    fshaderFG = new QGLShader(QGLShader::Fragment, this);
    vshaderP = new QGLShader(QGLShader::Vertex, this);
    fshaderP = new QGLShader(QGLShader::Fragment, this);
    vshaderT = new QGLShader(QGLShader::Vertex, this);
    fshaderT = new QGLShader(QGLShader::Fragment, this);

    vshaderFG->compileSourceFile("://vshaderFlatGouraud.glsl");
    fshaderFG->compileSourceFile("://fshaderFlatGouraud.glsl");
    vshaderP->compileSourceFile("://vshaderPhong.glsl");
    fshaderP->compileSourceFile("://fshaderPhong.glsl");
    vshaderT->compileSourceFile("://vshaderToon.glsl");
    fshaderT->compileSourceFile("://fshaderToon.glsl");

    program = new QGLShaderProgram(this);

    if(tipoShading == "Flat" || tipoShading == "Gouraud"){
        program->addShader(vshaderFG);
        program->addShader(fshaderFG);
    }else if(tipoShading == "Phong"){
        program->addShader(vshaderP);
        program->addShader(fshaderP);
    }else{
        program->addShader(vshaderT);
        program->addShader(fshaderT);
    }

    program->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("vColor", PROGRAM_COLOR_ATTRIBUTE);


    // muntatge del shader en el pipeline gràfic per a ser usat
    program->link();

    // unió del shader al pipeline gràfic
    program->bind();
}
```

#### `GLWidget::changeShadersGPU`

S'ha implementat aquest mètode per canviar els shaders usats pel programa shader.
Quan es crida, s'esborraran els shaders que hi havien carregats en aquell moment,
i s'afegiran els indicats en aquell moment per la variable `tipoShading`.

```c
void GLWidget::changeShadersGPU()
{
    program->removeAllShaders();

    if(this->tipoShading == "Flat" || this->tipoShading =="Gouraud"){
        program->addShader(vshaderFG);
        program->addShader(fshaderFG);
    }else if(this->tipoShading == "Phong"){
        program->addShader(vshaderP);
        program->addShader(fshaderP);
    }else{   
        program->addShader(vshaderT);
        program->addShader(fshaderT);
    }
    program->link();
    program->bind();
}
```

#### `GLWidget::reinit`

Aquest mètode ens permet reiniciar l'escena quan es produeix un canvi de shader
o si es vol renderitzar l'escena amb textures, a més dels materials.

```c
void GLWidget::reinit(QString Shading, bool Text){
    QString tipoShadingOld = tipoShading;
    esc->conTextura = Text;
    if(tipoShading != Shading){
        if(((tipoShading == "Flat" || tipoShading == "Gouraud") && (Shading != "Flat" && Shading != "Gouraud")) ||
              ((tipoShading != "Flat" && tipoShading != "Gouraud"))  ){
            std::cout << "CAMBIO DE SHADER: " << Shading.toStdString() << std::endl;
            tipoShading = Shading;
            changeShadersGPU();
        }
        if((tipoShadingOld == "Flat" && Shading != "Flat") || (tipoShadingOld != "Flat" && Shading == "Flat")){
            std::cout << "NEW NORMALS "<< Shading.toStdString() << std::endl;
            tipoShading = Shading;
            newSalaBillar();
        }else{
            std::cout<< "MISMAS NORMALES: " << Shading.toStdString() << std::endl;
            tipoShading = Shading;
        }
    }
}
```

En el cas que es canvii de `Flat` a qualsevol altra shader, cal tornar a
recalcular les normals de l'escena per a calcular la il·luminació correctament,
i en el cas de tornar al `Flat` shader, es tornen a recalcular també.

#### `GLWidget::keyPressEvent`

Per últim, es modifica aquest mètode per seleccionar quin shader es vol usar.

```c
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // ...
    switch ( event->key() )
    {
        case Qt::Key_1:
            if (event->modifiers() & Qt::ShiftModifier){
                reinit("Flat", true);
                break;
            }
            reinit("Flat", false);
            break;
        case Qt::Key_Exclam:
            reinit("Flat", true);
            break;
        case Qt::Key_2:
            if (event->modifiers() & Qt::ShiftModifier){
                reinit("Gouraud", true);
                break;
            }
            reinit("Gouraud", false);
            break;
        case Qt::Key_QuoteDbl:
            reinit("Gouraud", true);
            break;
        case Qt::Key_3:
            if (event->modifiers() & Qt::ShiftModifier){
                reinit("Phong", true);
                break;
            }
            reinit("Phong", false);
            break;
        case Qt::Key_periodcentered:
            reinit("Phong", true);
            break;
        case Qt::Key_4:
            if (event->modifiers() & Qt::ShiftModifier){
                reinit("Toon", true);
                break;
            }
            reinit("Toon", false);
            break;
        case Qt::Key_Dollar:
            reinit("Toon", true);
            break;
        //...
    }
    // ...
}
```

### 3.5 Els shaders

Ja tenim tot preparat per implementar els models d'il·luminació de manats a la
pràctica. 

A continuació, comentarem sobre algunes estructures que s'usen en els shaders, i
passarem després a comentar detalls sobre els shaders i a mostrar la implementació
usada en aquesta entrega.

#### 3.5.1 Estructura `tipusLlum`

Aquesta estructura dins als diferents shaders guarda els diferents paràmetres
d'una llum a l'escena. Donat que la nostra escena conté tres llums, en tots els
shaders, excepte el `Toon`, es passen tres instàncies de `tipusLlum`.

```c
uniform tipusLlum light1;
uniform tipusLlum light2;
uniform tipusLlum light3;
```

#### 3.5.2 Estructura `tipusMaterial`

De la mateixa manera que es passa la llum, per a cada objecte de l'escena s'ha
enviat la seva definició de material que es defineix en aquesta estructura.

#### 3.5.3 Altres variables `uniform`es

A més, s'han definit el paràmetre de llum ambiental global de l'escena, i el flag
booleà per indicar si es vol pintar la textura o no.

```c
uniform vec4 LuzAmbiente;
uniform bool conTextura;
```

#### 3.5.4 `Flat` i `Gouraud` shadings

Donat que el model d'il·luminació de tots dos shaders es calcula de la mateixa
manera, però només canvia les normals per generar un degradat més "poligonat",
si és `Flat`, o més suau, si és `Gouraud`, que ja s'han calculat en la generació
de triangles a la clase `Bola`, per exemple.

```glsl
// vshaderFlatGouraud.glsl

IN vec4 vPosition;
IN vec3 vNormal;
IN vec2 vCoordTexture;

OUT vec4 color;
OUT vec2 v_texcoord;

uniform mat4 model_view;
uniform mat4 projection;

struct tipusLlum{
    vec4 gpuLightPosition;
    vec4 gpuLightDirection;
    vec4 gpuIa;
    vec4 gpuId;
    vec4 gpuIs;
    float gpuBeamAngle;
    float gpuAtt_constant;
    float gpuAtt_linear;
    float gpuAtt_quadratic;
};

uniform tipusLlum light1;
uniform tipusLlum light2;
uniform tipusLlum light3;
uniform vec4 LuzAmbiente;

struct tipusMaterial{
    vec4 cA;
    vec4 cD;
    vec4 cS;
    float coefRefEsp;
};

uniform tipusMaterial material;
uniform bool conTextura;

// 3 luces
void main()
{
  gl_Position = projection * model_view * vPosition;
  //gl_Position = gl_Position/gl_Position.w;
  gl_Position[3] = 1.0;

  vec3 pos = (model_view * vPosition).xyz;
  vec3 L1 = normalize( (model_view * light1.gpuLightPosition).xyz - pos );
  vec3 L2 = normalize( (model_view * light2.gpuLightPosition).xyz - pos );
  vec3 L3 = normalize( (model_view * light3.gpuLightPosition).xyz - pos );
  vec3 E = normalize( -pos );
  vec3 H1 = normalize( L1 + E );
  vec3 H2 = normalize( L2 + E );
  vec3 H3 = normalize( L3 + E );
  vec3 N = normalize( model_view*vec4(vNormal, 0.0) ).xyz;
  float distance1 = length(light1.gpuLightPosition - vPosition);
  float distance2 = length(light2.gpuLightPosition - vPosition);
  float distance3 = length(light3.gpuLightPosition - vPosition);

  vec4 ambient1 = light1.gpuIa * material.cA;
  vec4 diffuse1 = light1.gpuId * material.cD * max( dot(L1, N), 0.0 );
  vec4 specular1 = max(pow(max(dot(N, H1), 0.0), material.coefRefEsp) * light1.gpuIs * material.cS , 0.0);
  float attDist1 = 1.0/(light1.gpuAtt_constant + light1.gpuAtt_linear * distance1 + light1.gpuAtt_quadratic * pow(distance1,2.0) );
  vec4 Ilum1 = attDist1 * (ambient1 + diffuse1 + specular1);

  vec4 ambient2 = light2.gpuIa * material.cA;
  vec4 diffuse2 = light2.gpuId * material.cD * max( dot(L2, N), 0.0 );
  vec4 specular2 = max(pow(max(dot(N, H2), 0.0), material.coefRefEsp) * light2.gpuIs * material.cS , 0.0);
  float attDist2 = 1.0/(light2.gpuAtt_constant + light2.gpuAtt_linear * distance2 + light2.gpuAtt_quadratic * pow(distance2,2.0) );
  vec4 Ilum2 = attDist2 * (ambient2 + diffuse2 + specular2);

  vec4 ambient3 = light3.gpuIa * material.cA;
  vec4 diffuse3 = light3.gpuId * material.cD * max( dot(L3, N), 0.0 );
  vec4 specular3 = max(pow(max(dot(N, H3), 0.0), material.coefRefEsp) * light3.gpuIs * material.cS , 0.0);
  float attDist3 = 1.0/(light3.gpuAtt_constant + light3.gpuAtt_linear * distance3 + light3.gpuAtt_quadratic * pow(distance3,2.0) );
  vec4 Ilum3 = attDist3 * (ambient3 + diffuse3 + specular3);

  color =  LuzAmbiente * material.cA  + Ilum1 + Ilum2 + Ilum3;
  color[3] = 1.0;
  v_texcoord = vCoordTexture;
}
```

Com es pot veure, en el vertex shader es calcula el color que serà interpolat
i passat al fragment shader, a continuació:

```c
// fshaderFlatGouraud.glsl

IN vec4 color;
IN vec2 v_texcoord;

uniform sampler2D texMap;
uniform bool conTextura;

void main()
{
    if(conTextura == true){
        gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
    }else{
        gl_FragColor = color;
    }
}
```

#### 3.5.5 `Phong` shading

A diferència dels shadings anteriors, en comptes d'aplicar el model d'il·luminació
Blinn-Phong en l'etapa de processament de vèrtex, una vegada han estat interpolats,
i el fragment shader s'encarrega de pintar cada píxel, s'aplica el model en aquest
punt.

Tot i així, per aplicar aquest model al fragment shader, es requereix fer alguns
càlculs sobre la posició de les llums i ser passats fragment.

```c
// vshaderPhong.glsl

IN vec4 vPosition;
IN vec3 vNormal;
IN vec2 vCoordTexture;
OUT vec2 v_texcoord;

OUT vec3 pos;
OUT vec3 L1;
OUT vec3 L2;
OUT vec3 L3;
OUT vec3 N;
OUT float distance1;
OUT float distance2;
OUT float distance3;

uniform mat4 model_view;
uniform mat4 projection;

struct tipusLlum{
    vec4 gpuLightPosition;
    vec4 gpuLightDirection;
    vec4 gpuIa;
    vec4 gpuId;
    vec4 gpuIs;
    float gpuBeamAngle;
    float gpuAtt_constant;
    float gpuAtt_linear;
    float gpuAtt_quadratic;
};

uniform tipusLlum light1;
uniform tipusLlum light2;
uniform tipusLlum light3;
uniform vec4 LuzAmbiente;

struct tipusMaterial{
    vec4 cA;
    vec4 cD;
    vec4 cS;
    float coefRefEsp;
};

uniform tipusMaterial material;
uniform bool conTextura;


void main()
{
  gl_Position = projection * model_view * vPosition;
  //gl_Position = gl_Position/gl_Position.w;
  gl_Position[3] = 1.0;

  pos = (model_view * vPosition).xyz;
  L1 = normalize( (model_view * light1.gpuLightPosition).xyz - pos );
  L2 = normalize( (model_view * light2.gpuLightPosition).xyz - pos );
  L3 = normalize( (model_view * light3.gpuLightPosition).xyz - pos );
  N = normalize( model_view * vec4(vNormal, 0.0) ).xyz;

  distance1 = length(light1.gpuLightPosition - vPosition);
  distance2 = length(light2.gpuLightPosition - vPosition);
  distance3 = length(light3.gpuLightPosition - vPosition);

  v_texcoord = vCoordTexture;

}
```

Una vegada calculades les normals de cadascun dels vèrtex, aquestes són interpolades
i enviades al fragment shader, a on ja es pot calcular el color corresponent al
píxel:

```c
// fshaderPhong.glsl

IN vec2 v_texcoord;

IN vec3 pos;
IN vec3 L1;
IN vec3 L2;
IN vec3 L3;
IN vec3 N;
IN float distance1;
IN float distance2;
IN float distance3;

struct tipusLlum{
    vec4 gpuLightPosition;
    vec4 gpuLightDirection;
    vec4 gpuIa;
    vec4 gpuId;
    vec4 gpuIs;
    float gpuBeamAngle;
    float gpuAtt_constant;
    float gpuAtt_linear;
    float gpuAtt_quadratic;
};

uniform tipusLlum light1;
uniform tipusLlum light2;
uniform tipusLlum light3;
uniform vec4 LuzAmbiente;

struct tipusMaterial{
    vec4 cA;
    vec4 cD;
    vec4 cS;
    float coefRefEsp;
};

uniform tipusMaterial material;


uniform sampler2D texMap;
uniform bool conTextura;

void main()
{
  vec3 E = normalize( -pos );//vector de la pos al eye. En coordenadas de camara el eye esta en el origen
  vec3 H1 = normalize( L1 + E );
  vec3 H2 = normalize( L2 + E );
  vec3 H3 = normalize( L3 + E );

  vec4 ambient1 = light1.gpuIa * material.cA;
  vec4 diffuse1 = light1.gpuId * material.cD * max( dot(L1, N), 0.0 );
  vec4 specular1 = max(pow(max(dot(N, H1), 0.0), material.coefRefEsp) * light1.gpuIs * material.cS , 0.0);
  float attDist1 = 1.0/(light1.gpuAtt_constant + light1.gpuAtt_linear * distance1 + light1.gpuAtt_quadratic * pow(distance1,2.0) );
  vec4 Ilum1 = attDist1 * (ambient1 + diffuse1 + specular1);

  vec4 ambient2 = light2.gpuIa * material.cA;
  vec4 diffuse2 = light2.gpuId * material.cD * max( dot(L2, N), 0.0 );
  vec4 specular2 = max(pow(max(dot(N, H2), 0.0), material.coefRefEsp) * light2.gpuIs * material.cS , 0.0);
  float attDist2 = 1.0/(light2.gpuAtt_constant + light2.gpuAtt_linear * distance2 + light2.gpuAtt_quadratic * pow(distance2,2.0) );
  vec4 Ilum2 = attDist2 * (ambient2 + diffuse2 + specular2);

  vec4 ambient3 = light3.gpuIa * material.cA;
  vec4 diffuse3 = light3.gpuId * material.cD * max( dot(L3, N), 0.0 );
  vec4 specular3 = max(pow(max(dot(N, H3), 0.0), material.coefRefEsp) * light3.gpuIs * material.cS , 0.0);
  float attDist3 = 1.0/(light3.gpuAtt_constant + light3.gpuAtt_linear * distance3 + light3.gpuAtt_quadratic * pow(distance3,2.0) );
  vec4 Ilum3 = attDist3 * (ambient3 + diffuse3 + specular3);

  vec4 color =  LuzAmbiente * material.cA  + Ilum1 + Ilum2 + Ilum3;
  color[3] = 1.0;

   if(conTextura == true){
       gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
   }else{
       gl_FragColor = color;
   }
}
```

#### 3.5.6 `Toon` shading

En aquest shader es discretitza el color calculat per donar la sensació de ser
un color dibuixat, en comptes de ser fotorealista com els shadings anteriors.

A diferència dels shadings anteriors, només utilitzem la primera de les tres
llums de l'escena.

```c
// vshaderToon.glsl

IN vec4 vPosition;
IN vec3 vNormal;
IN vec2 vCoordTexture;

OUT vec3 Normal;
OUT vec2 v_texcoord;


uniform mat4 model_view;
uniform mat4 projection;

struct tipusLlum{
    vec4 gpuLightPosition;
    vec4 gpuLightDirection;
    vec4 gpuIa;
    vec4 gpuId;
    vec4 gpuIs;
    float gpuBeamAngle;
    float gpuAtt_constant;
    float gpuAtt_linear;
    float gpuAtt_quadratic;
};

uniform tipusLlum light1;
uniform tipusLlum light2;
uniform tipusLlum light3;
uniform vec4 LuzAmbiente;


uniform bool conTextura;


void main()
{
  gl_Position = projection * model_view * vPosition;
  //gl_Position = gl_Position/gl_Position.w;
  gl_Position[3] = 1.0;

  Normal = normalize(vNormal);
  v_texcoord = vCoordTexture;

}

```

En el fragment shader, calculem el color discretitzat en funció de la llum
direccional que apunta cap al centre de coordenades del mon:

```c
// fshaderToon.glsl

IN vec3 Normal;
IN vec2 v_texcoord;

struct tipusLlum{
    vec4 gpuLightPosition;
    vec4 gpuLightDirection;
    vec4 gpuIa;
    vec4 gpuId;
    vec4 gpuIs;
    float gpuBeamAngle;
    float gpuAtt_constant;
    float gpuAtt_linear;
    float gpuAtt_quadratic;
};

uniform tipusLlum light1;

uniform sampler2D texMap;
uniform bool conTextura;

void main()
{
    float intensity;
    vec4 color;

    intensity = dot(normalize(light1.gpuLightPosition.xyz), Normal);//si consideramos luz direccional que apunta al origen de coord. en el world

    if (intensity > 0.95)
        color = vec4(0.5, 1.0, 0.5, 1.0);
    else if (intensity > 0.5)
        color = vec4(0.3, 0.6, 0.3, 1.0);
    else if (intensity > 0.25)
        color = vec4(0.2, 0.4, 0.2, 1.0);
    else
        color = vec4(0.1, 0.2, 0.1, 1.0);
        
    if(conTextura == true){
        gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
    }else{
        gl_FragColor = color;
    }
}
```

#### 3.5.7 Texturització i il·luminació

Per últim, anem a fer un petit incís sobre cóm hem combinat les textures amb
la il·luminació i els materials de la textura.

A qualsevol dels fragments shaders es pot veure el següent fragment de codi:

```glsl
// ...
uniform bool conTextura;
// ...

void main()
{
    // ...        
    if(conTextura == true){
        gl_FragColor = 0.25 * color + 0.75 * texture2D(texMap, v_texcoord);
    }else{
        gl_FragColor = color;
    }
}
```

Com s'ha esmentat més abans, utilitzem la variable uniforme `conTextura` per
decidir si s'ha d'aplicar o no la textura al píxel que s'està calculant.

El color calculat, ja sigui des del vertex shader pasat al fragment shader, o 
calculat des del propi fragment shader, es combina amb el color de la textura,
en una proporció de 25% del color calculat a partir de la il·luminació i del
material propi de l'objecte amb un 75% del color de la textura, tal com es
demana a l'enunciat de la pràctica.
