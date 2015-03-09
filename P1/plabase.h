#ifndef PLABASE_H
#define PLABASE_H

#include <objecte.h>

const int NumVertices = 6; //(1 faces)*(2 triangles/face)*(3 vertices/triangle)

class PlaBase : public Objecte
{
public:
    PlaBase();
    virtual void make();
    ~PlaBase();
private:
    int a; //amplada
    int p; //profunditat
    GLdouble xorig, zorig;

    point4 vertices[4];
    color4 vertex_colors[4];

    point4 points[NumVertices];
    color4 colors[NumVertices];

    void quad( int a, int b, int c, int d );
};

#endif // PLABASE_H
