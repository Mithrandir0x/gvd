#ifndef LLUM_H
#define LLUM_H

#include <Common.h>
#include <QGLShaderProgram>

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

#endif // LLUM_H
