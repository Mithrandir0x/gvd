#ifndef VECTOROBJECTES_H
#define VECTOROBJECTES_H

#include <objecte.h>
#include <rectangle.h>
#include <vector>
#include <iostream>

class VectorObjectes
{
    vector<Objecte *> conjunt;

public:
    VectorObjectes();
    void afegirObjecte(Objecte *o);
    void imprimirConjunt();

    ~VectorObjectes();
};

#endif // VECTOROBJECTES_H
