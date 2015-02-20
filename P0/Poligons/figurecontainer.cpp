#include "figurecontainer.h"

FigureContainer::FigureContainer()
{

}

void FigureContainer::addFigure(Figure *figure)
{
    this->figures.push_back(figure);
}

double FigureContainer::getPerimeters()
{
    double sum = 0.0;

    for ( Figure *f : this->figures ) {
        sum += f->getPerimeter();
    }

    return sum;
}

FigureContainer::~FigureContainer()
{

}

