#include "figure.h"

#ifndef SQUARE_H
#define SQUARE_H


class Square:public Figure
{
private:
    float edge;
public:
    Square();
    Square(float edgeSize);
    virtual float getPerimeter();
    ~Square();
};

#endif // SQUARE_H
