#ifndef MATERIAL_H
#define MATERIAL_H

#include <Common.h>
#include <math.h>
#include <QGLShaderProgram>

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



#endif // MATERIAL_H
