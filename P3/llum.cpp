#include "llum.h"
using namespace std;

Llum::Llum()
{
}

Llum::Llum(vec4 pos, vec4 dir, vec4 iA, vec4 iD, vec4 iS, float bAngle, float atConstant, float atLin, float atQuad, bool on){

    lightPosition = pos;
    lightDirection = dir;
    Ia = iA;
    Id = iD;
    Is = iS;
    bmAngle = bAngle;
    at_constant = atConstant;
    at_linear = atLin;
    at_quadratic = atQuad;
    ON = on;
}

Llum::~Llum()
{

}

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

