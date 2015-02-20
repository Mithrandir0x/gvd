#include <iostream>
#include <rectangle.h>

using namespace std;

int main()
{
    Rectangle r0;
    Rectangle r1 (2.0, 5.0);
    Rectangle *r2;

    r2 = new Rectangle();

    cout<< "Area de r0: " << r0.calculArea() << endl;
    cout<< "Area de r1: " << r1.calculArea() << endl;
    cout<< "Area de r2: " << r2->calculArea()<<endl;

    //delete r2;

    Rectangle conjunt[3];
    cout<< "Area de conjunt de 0: " << conjunt[0].calculArea() << endl;
    return 0;

    Rectangle **conjunt2;

    conjunt2 = new Rectangle*[3];
    conjunt2[0] = new Rectangle();

}

