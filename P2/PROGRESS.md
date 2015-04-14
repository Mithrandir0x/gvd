
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

## 3 Modificació de la classe càmera

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

En primer lloc, es calcula el View Reference Point a partir de la capça
contenidora passada per paràmetre:

```c
vs.vrp[0] = capsaMinima.pmin[0] + ( capsaMinima.a / 2. );
vs.vrp[1] = capsaMinima.pmin[1] + ( capsaMinima.h / 2. );
vs.vrp[2] = capsaMinima.pmin[2] + ( capsaMinima.p / 2. );
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
> - Les coordenades de la projecció paral·lela estan forçades. S'hauria de
>   mirar si s'han de calcular respecte alguna variable.
> - Les coordenades de la projecció perspectiva estan malament. S'han de
>   corregir

En funció del tipus de projecció que s'estigui utilitzant, la matriu de
projecció canvia:

```c
proj = identity();

if ( piram.proj == PARALLELA ) {
    proj =
        Ortho(-1., -1., -1., 1., 1., -1.) *
        proj;
} else if ( piram.proj == PERSPECTIVA ) {
    proj =
        Frustum(-1., -1., -1., 1., 1., -1.) *
        proj;
}
```
#### `Camera::CalculWindow`

> NO IMPLEMENTAT

## 4 Modificació de la classe Escena per incloure la càmera general

### 4.1 Inclusió d'un nou atribut `camGeneral` a la classe Escena

A `escena.h`, s'ha inclós la següent capçalera:

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
s'han afegit les següents línies de codi al constructor, després de construir la
instància de la càmera:

```c
// Situem "l'objectiu" que mira la càmera a (0, 0, 0), a una distància de 20
// respecte l'eix de la Y.
camGeneral->vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
camGeneral->vs.obs = camGeneral->CalculObs(camGeneral->vs.vrp, 20, 0, 90.0);
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
        Capsa3D capsaMinima;
        camGeneral->ini(wd.a, wd.h, capsaMinima);
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
