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
