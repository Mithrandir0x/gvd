#ifndef TAULABILLAR_H
#define TAULABILLAR_H

#include <objecte.h>

class TaulaBillar : public Objecte
{
public:
    TaulaBillar(QString n);
    ~TaulaBillar();

private:

    //static const int NumVerticesF = 100000;
    static const int NumVerticesF = 87288;//numero real de vertices para calcular bien la capsa
    static const int NumCares = 300000;

};

#endif // TAULABILLAR_H
