#include "rectangle.h"

Rectangle::Rectangle()
{
    cout<<"Constructor de Rectangle buit"<<endl;
    base = 1.0;
    altura = 1.0;
}

Rectangle::Rectangle(float base, float altura):Objecte(base, altura) {
    cout<<"Constructor de Rectangle amb parametres"<<endl;
    this->base = base;
    this->altura = altura;
}

float Rectangle::calculArea() {
    return Objecte::calculArea();
}

void Rectangle::imprimir() {
    cout<<"base: "<<base<<" alcada: "<<altura<<endl;
}

void Rectangle::copiaRectangle(Rectangle &r) {
    r.altura = altura;
    r.base = base;
    cout<<"r: alcada "<<r.altura<<" amplada "<<r.base<<endl;
    cout<<"this: alcada "<<altura<<" amplada "<<base<<endl;

}

Rectangle::~Rectangle() {
    cout<<"Destructor de rectangle"<<endl;

}
