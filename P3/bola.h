#ifndef BOLA_H
#define BOLA_H

#include <Common.h>
#include "objecte.h"
#include <QOpenGLTexture>
#include <math.h>

const int NumVerticesBola = 12288; //(4 triangles)*(4 triangles/(triangle*iteration))^(5 iterations)*(3 vertices/triangle)

class Bola : public Objecte
{
public:

    Bola(double x0, double y0, double z0, double r, double R, double G, double B, Material *material, QString tipoShading, QString numBola);
    ~Bola();
    void make(double x0, double y0, double z0, double r, QString numBola);
    color4 color; //color de la bola

private:
    //color4 color; //color de la bola
    void triangle(const point4& a, const point4& b, const point4& c );
    void divide_triangle(const point4& a, const point4& b, const point4& c, int cont);
    vec4 normalize(const vec4 &p);
    void initTextura(QString numBola);//añadido

    double r; // radio
    int NumIteracionsEsfera = 5;
    QString tipoShading;
};

#endif // BOLA_H
