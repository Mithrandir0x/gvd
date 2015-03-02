#ifndef FIGURECONTAINER_H
#define FIGURECONTAINER_H

#include <iostream>
#include <figure.h>
#include <vector>

using namespace std;

class FigureContainer
{
private:
    vector<Figure *> figures;
public:
    FigureContainer();
    void addFigure(Figure *);
    double getPerimeters();
    ~FigureContainer();
};

#endif // FIGURECONTAINER_H
