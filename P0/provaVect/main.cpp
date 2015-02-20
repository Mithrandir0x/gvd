#include <iostream>
#include <rectangle.h>
#include <conjuntobjectes.h>
#include <vectorobjectes.h>

using namespace std;

int main()
{

// Opcio 1: Array de Rectangles

    std::cout<<"Array de rectangles"<<std::endl;

    Rectangle *conjuntR;
    conjuntR = new Rectangle[3];

    conjuntR[0].imprimir();
    delete[] conjuntR;

// Opcio 2: Array objectes
    std::cout<<"Array de punters a Objectes"<<std::endl;

    Objecte *conjunt[3];

    Rectangle r21;
    Rectangle *r22 = new Rectangle ();
    conjunt[0] = new Rectangle(8.0, 5.0);
    conjunt[0]->imprimir();
    conjunt[1] = &r21;
    conjunt[2] = r22;

    delete (Rectangle *)conjunt[0];
    delete (Rectangle *)conjunt[2];

// Opcio 3: Conjunt Objectes amb classe que codifica l'array
    std::cout<<"Classe de conjunt d'' Objectes basats en array"<<endl;

    ConjuntObjectes *c;

    c = new ConjuntObjectes();

    //Rectangle r0(8.0, 20.0);
    Rectangle *r1;
    r1 = new Rectangle(9.0, 10.0);
    //c->afegirObjecte(&r0);
    //c->imprimirConjunt();

    c->afegirObjecte(r1);
    c->imprimirConjunt();
    delete c;

// Opcio 4.1: vector d'objectes
    std::cout<<"Vector  de punters a Objectes"<<std::endl;

    vector<Objecte *> v0;

//    Rectangle r0;
    Rectangle *r01 = new Rectangle(2.0, 2.0);

//    v0.push_back(&r0);
    v0.push_back(r01);

    std::cout<<"Vector d'objectes"<<std::endl;
    for (unsigned int i=0; i<v0.size(); i++) {
        v0[i]->imprimir();
    }
    std::cout << "Iterador"<<std::endl;
    for (std::vector<Objecte *>::iterator it = v0.begin() ; it != v0.end(); ++it) {
        (*it)->imprimir();
    }


    std::cout << "For each"<<std::endl;
      for (Objecte *o: v0) {
        o->imprimir();
    }

    delete r01;
    v0.clear();
    std::cout<<"Vector d'objectes: despres del clear:"<<v0.size()<<std::endl;
    for (unsigned int i=0; i<v0.size(); i++) {
        v0[i]->imprimir();
    }



// Opcio 4.2: vector d'objectes
    std::cout<<"Classe de vector de punters a Objectes"<<std::endl;

    VectorObjectes *v;


    v = new VectorObjectes();
    r1 = new Rectangle(10.0, 20.0);
    v->afegirObjecte(r1);

    v->imprimirConjunt();
    delete v;

    // Opcio 5: pas per referencia

    Rectangle r;
    Rectangle r2;
    Rectangle *r3 = new Rectangle();

    r.copiaRectangle(r2);
    r2.imprimir();

    r.copiaRectangle(*r3);

    r3->imprimir();
    delete r3;
    cout<<"Final"<<endl;
    return 0;
}

