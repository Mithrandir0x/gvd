#ifndef LLUM_H
#define LLUM_H

#include <Common.h>

#include <QGLShaderProgram>

/* AMBIENT IS POINT-LIGHT */
typedef enum { AMBIENT = 0, DIRECCIONAL = 1, SPOTLIGHT = 2 } TipusLlum;

typedef struct {


} gl_llum;

class Llum
{
public:
    TipusLlum tipusLlum;

    vec4 posicioLlum;
    vec3 difusa;
    float coefA;

    Llum();
    ~Llum();

    void toGpu(QGLShaderProgram *program);
private:
    struct {
        GLuint posicio;
        GLuint ld;
        GLuint a;
    } gl_IdLlum;
};

#endif // LLUM_H
