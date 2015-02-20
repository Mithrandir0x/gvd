#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>

using namespace std;

class Rectangle
{
private:
    float base;
    float altura;
public:
    Rectangle();
    Rectangle(float b, float h);
    float calculArea();
    ~Rectangle();
};

#endif // RECTANGLE_H
