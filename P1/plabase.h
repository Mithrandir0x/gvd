#ifndef PLABASE_H
#define PLABASE_H
#include <objecte.h>

const int NumVertices = 6;

class PlaBase : public Objecte
{
public:
    PlaBase();
    ~PlaBase();
private:
    int a; //amplada
    int p; //profunditat
    GLdouble xorig, zorig;

    point4 vertices[4];
    color4 vertex_colors[4];

    point4 points[NumVertices];
    color4 colors[NumVertices];
};

#endif // PLABASE_H
