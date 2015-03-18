#ifndef BOLA_H
#define BOLA_H

#include <Common.h>
#include "objecte.h"

const int NumIteracions = 3;
const int NumVerticesBola = 4 * pow(4.0, NumIteracions) * 3; //(4 triangles)*(4 triangles/(triangle*iteration))^(5 iterations)*(3 vertices/triangle)


class Bola : public Objecte
{
public:
    Bola(double x0, double y0, double z0, double r, double R, double G, double B);
    ~Bola();
    void make(double x0, double y0, double z0, double r);

private:
    color4 color; //color de la bola
    void triangle(const point4& a, const point4& b, const point4& c );
    void divide_triangle(const point4& a, const point4& b, const point4& c, int cont);
    vec4 normalize(const vec4 &p);

    double r; // radio
    int NumIteracionsEsfera = NumIteracions;
};

#endif // BOLA_H
