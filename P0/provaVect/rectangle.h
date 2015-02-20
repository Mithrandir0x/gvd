#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>
#include <objecte.h>


using namespace std;

class Rectangle: public Objecte
{
private:
    float base;
    float altura;
public:
    Rectangle();
    Rectangle(float b, float h);
    virtual float calculArea();
    virtual void imprimir();
    void copiaRectangle(Rectangle &r);
    virtual ~Rectangle();
};

#endif // RECTANGLE_H
