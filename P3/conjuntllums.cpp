#include "conjuntllums.h"

ConjuntLlums::ConjuntLlums()
{
    vec4 pos = vec4(0.0, 0.1, 1.0, 1);
    vec4 dir = vec4(0.0, 0.1, 1.0 , 0);
    vec4 ia = vec4 (0.2, 0.2, 0.2, 1);
    vec4 id = vec4 (0.2, 0.1, 0.5, 1);
    vec4 is = vec4 (0.1, 0.1, 0.1, 1);
    float bAngle = 60.0;
    float atConst = 0.2;
    float atLin = 0.2;
    float atQuad = 0.2;
    bool on = true;
    luz1 = new Llum(pos, dir, ia, id, is, bAngle, atConst, atLin, atQuad, on);

    pos = vec4(2.0, 0.1, 1.0, 1);
    dir = vec4(2.0, 0.1, 1.0, 0);
    ia = vec4 (0.2, 0.2, 0.2, 1);
    id = vec4 (0.2, 0.1, 0.5, 1);
    is = vec4 (0.1, 0.1, 0.1, 1);
    bAngle = 60.0;
    atConst = 0.2;
    atLin = 0.2;
    atQuad = 0.2;
    on = true;
    luz2 = new Llum(pos, dir, ia, id, is, bAngle, atConst, atLin, atQuad, on);

    pos = vec4(-2.0, 0.1, 1.0, 1);
    dir = vec4(-2.0, 0.1, 1.0, 0);
    ia = vec4 (0.2, 0.2, 0.2, 1);
    id = vec4 (0.2, 0.1, 0.5, 1);
    is = vec4 (0.1, 0.1, 0.1, 1);
    bAngle = 60.0;
    atConst = 0.2;
    atLin = 0.2;
    atQuad = 0.2;
    on = true;
    luz3 = new Llum(pos, dir, ia, id, is, bAngle, atConst, atLin, atQuad, on);
}


void ConjuntLlums::toGPU(QGLShaderProgram *program)
{
     if(luz1->ON == true)//solo a la GPU si encendida
     {
       luz1->toGPU(program, (const char*) "light1");
     }
     if(luz2->ON == true)
     {
       luz2->toGPU(program, (const char*) "light2");
     }
     if(luz3->ON == true)
     {
       luz3->toGPU(program, (const char*) "light3");
     }
}

