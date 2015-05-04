#include "llum.h"

#include

Llum::Llum()
{

}

Llum::~Llum()
{

}

void Llum::toGpu(QGLShaderProgram *program)
{
    gl_IdLlum.posicio = program->uniformLocation("light.LightPosition");
    gl_IdLlum.ld = program->uniformLocation("light.Ld");
    //gl_IdLlum.a = program->uniformLocation("light.coef_a");

    glUniform4fv(gl_IdLlum.posicio, 1, posicioLlum);
    glUniform3fv(gl_IdLlum.ld, 1, difusa);
    //glUniform1f(gl_IdLlum.a, coefA);
}
