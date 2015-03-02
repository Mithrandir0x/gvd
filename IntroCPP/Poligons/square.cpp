#include <square.h>

Square::Square()
{
    this->edge = 1;
}

Square::Square(float edgeSize)
{
    this->edge = edgeSize;
}

float Square::getPerimeter()
{
    return 4 * this->edge;
}

Square::~Square()
{

}

