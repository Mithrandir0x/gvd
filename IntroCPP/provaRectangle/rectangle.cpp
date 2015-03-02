#include "rectangle.h"

Rectangle::Rectangle()
{
    cout<<"Constructor de Rectangle buit"<<endl;
    base = 1.0;
    altura = 1.0;
}

Rectangle::Rectangle(float base, float altura) {
    cout<<"Constructor de Rectangle amb parametres"<<endl;
    this->base = base;
    this->altura = altura;
}

float Rectangle::calculArea() {
    return base*altura;
    
}

Rectangle::~Rectangle() {
    cout<<"Destructor de rectangle"<<endl;

}
