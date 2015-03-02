#include <iostream>
#include <rectangle.h>

using namespace std;

int main()
{
    cout << "Creant r0" << endl;
    Rectangle r0;
    cout << endl;

    cout << "Creant r1" << endl;
    Rectangle r1 (2.0, 5.0);
    cout << endl;

    cout << "Creant punter r2" << endl;
    Rectangle *r2;
    cout << endl;

    cout << "Assignant nova instancia de Rectangle a punter r2" << endl;
    r2 = new Rectangle();
    cout << endl;

    cout<< "Area de r0: " << r0.calculArea() << endl;
    cout<< "Area de r1: " << r1.calculArea() << endl;
    cout<< "Area de r2: " << r2->calculArea()<< endl;

    delete r2;
    return 0;
}

