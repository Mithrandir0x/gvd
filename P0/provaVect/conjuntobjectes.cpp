#include "conjuntobjectes.h"

ConjuntObjectes::ConjuntObjectes()
{
    nObjectes = 0;
}

void ConjuntObjectes::afegirObjecte(Objecte *o) {
    if (nObjectes<MAX_OBJECTES) {
        conjunt[nObjectes]= o;
        nObjectes++;
    }
}

void ConjuntObjectes::imprimirConjunt() {
    for (int i=0; i<nObjectes; i++) {
        conjunt[i]->imprimir();
    }
}

ConjuntObjectes::~ConjuntObjectes() {
    cout<<"Esborrant objectes"<<endl;
    for (int i=0; i<nObjectes; i++) {
        // delete conjunt[i];
        if (dynamic_cast<Rectangle*>(conjunt[i]))
                delete (Rectangle *)(conjunt[i]);
    }
}
