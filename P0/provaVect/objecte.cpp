#include "objecte.h"

Objecte::Objecte()
{
    cout<<"Crida al constructor d'Objecte"<<endl;
}


Objecte::Objecte(float base, float altura) {
    cout<<"Constructor de l'Objecte amb parametres"<<endl;
    this->base = base;
    this->altura = altura;
}

float Objecte::calculArea() {
    cout<<"Calcul Area de l'Objecte"<<endl;
    return base*altura;

}
Objecte::~Objecte() {
    cout<<"Destructor de objecte"<<endl;

}
