#include "material.h"

Material::Material()
{

}

Material::Material(vec4 cA, vec4 cD, vec4 cE, float coefRefEsp)
{
    this->cA = cA; //componente ambiente
    this->cD = cD; //componente difusa
    this->cE = cE; //componente especular
    this->coefRefEsp = coefRefEsp; // coef. de reflexion especular
}

Material::~Material()
{

}

void Material::toGPU(QGLShaderProgram *program)
{
    gl_IdMaterial.cA = program->uniformLocation("material.cA");
    glUniform4fv(gl_IdMaterial.cA, 1, cA);

    gl_IdMaterial.cD = program->uniformLocation("material.cD");
    glUniform4fv(gl_IdMaterial.cD, 1, cD);

    gl_IdMaterial.cE = program->uniformLocation("material.cE");
    glUniform4fv(gl_IdMaterial.cE, 1, cE);

    gl_IdMaterial.coefRefEsp = program->uniformLocation("material.coefRefEsp");
    glUniform1f(gl_IdMaterial.coefRefEsp, coefRefEsp);
}


