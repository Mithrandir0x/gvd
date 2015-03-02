#ifndef CONJUNTOBJECTES_H
#define CONJUNTOBJECTES_H

#define MAX_OBJECTES 10
#include <objecte.h>
#include <rectangle.h>

class ConjuntObjectes
{
private:
    Objecte *conjunt[MAX_OBJECTES];
    int      nObjectes;

public:
    ConjuntObjectes();
    void afegirObjecte(Objecte *o);
    void imprimirConjunt();
    ~ConjuntObjectes();
};

#endif // CONJUNTOBJECTES_H
