#include "vectorobjectes.h"

VectorObjectes::VectorObjectes()
{
    // no cal constructor ja que es reserva memòria en la declaració
}

void VectorObjectes::afegirObjecte(Objecte *o) {
        conjunt.push_back(o);
}

void VectorObjectes::imprimirConjunt() {
    std::cout << "Recorregut" << std::endl;

    for (unsigned int i=0; i<conjunt.size(); i++) {
        conjunt[i]->imprimir();
    }

    std::cout << "Iterador"<<std::endl;
    for (std::vector<Objecte *>::iterator it = conjunt.begin() ; it != conjunt.end(); ++it) {
        (*it)->imprimir();
    }


    std::cout << "For each"<<std::endl;
      for (Objecte *o: conjunt) {
        o->imprimir();
    }
}

VectorObjectes::~VectorObjectes() {
    std::cout<<"Esborrant objectes"<<std::endl;

    for (unsigned int i=0; i<conjunt.size(); i++) {
        // delete conjunt[i];
        if (dynamic_cast<Rectangle*>(conjunt[i]))
                delete (Rectangle *)(conjunt[i]);
    }
}
