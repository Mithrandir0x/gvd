#include <iostream>
#include <figurecontainer.h>
#include <figure.h>
#include <square.h>

using namespace std;

int main()
{
    Square *s = new Square(3.0);
    Square *s2 = new Square(2.0);
    FigureContainer *fc = new FigureContainer();

    fc->addFigure(s);
    fc->addFigure(s2);

    cout << "Perimetre quadrat: " << s->getPerimeter() << endl;
    cout << "Perimetre de tots els poligons: " << fc->getPerimeters() << endl;

    delete fc;
    delete s;

    return 0;
}

