#ifndef PLABASE_H
#define PLABASE_H

#include <objecte.h>
#include <material.h>

const int NumVertices = 6; //(1 faces)*(2 triangles/face)*(3 vertices/triangle)

class PlaBase : public Objecte
{
public:
    PlaBase(point4 v0, point4 v1, point4 v2, point4 v3, color4 cv0, color4 cv1, color4 cv2, color4 cv3, Material *mat);
    virtual void make();
    ~PlaBase();
private:
    int a; //amplada
    int p; //profunditat

    point4 vertices[4];
    color4 vertex_colors[4];

    void quad( int a, int b, int c, int d );
    void initTextura();
};

#endif // PLABASE_H
