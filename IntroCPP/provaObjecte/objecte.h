#ifndef OBJECTE_H
#define OBJECTE_H
#include <iostream>

using namespace std;

class Objecte
{
private:
    float base, altura;
public:
    Objecte();
    Objecte(float base, float alcada);
    virtual float calculArea();
    ~Objecte();
};

#endif // OBJECTE_H
