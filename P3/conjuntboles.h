#ifndef CONJUNTBOLES_H
#define CONJUNTBOLES_H

#include <bola.h>

class ConjuntBoles
{
public:
    ConjuntBoles(QString tipoShading);
    ~ConjuntBoles();

    vector<Bola*> listaConjuntBoles;
};

#endif // CONJUNTBOLES_H
