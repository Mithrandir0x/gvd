#include "conjuntboles.h"

typedef struct {
    double x;
    double y;
    double z;
} Color;

ConjuntBoles::ConjuntBoles()
{
    Capsa3D capsa;
    Bola *bola;
    double x = 0.0;
    double y = 0.03075;
    double z = 0.0;
    double r = 0.03075;
    double z1 = -0.5;
    double x2 =  0.0 - 3 * r;
    double z2 = -0.5 - 2 * r * cos(30*M_PI/180.0);
    double x3 =  0.0 - 4 * r;
    double z3 = -0.5 - 4 * r * cos(30*M_PI/180.0);
    double x4 =  0.0 - 5 * r;
    double z4 = -0.5 - 6 * r * cos(30*M_PI/180.0);
    double x5 =  0.0 - 6 * r;
    double z5 = -0.5 - 8 * r * cos(30*M_PI/180.0);

    Color color;

    mat4 m;
    QString numBola;

    for ( int i=0 ; i < 15 ; i++ ) {
        if ( i == 0 ) {
            z = z1;
            numBola = "1";
            color.x = 246.0 / 255.0;
            color.y = 171.0 / 255.0;
            color.z = 52.0  / 255.0;
        } else if ( i > 0 && i < 3 ) {
            x = x2 + 2 * i * r;
            z= z2;
            if ( i == 1 ) {
                numBola = "2";
                color.x = 18.0  / 255.0;
                color.y = 29.0  / 255.0;
                color.z = 109.0 / 255.0;
            }
            if ( i == 2 ) {
                numBola = "3";
                color.x = 64.0  / 255.0;
                color.y = 162.0  / 255.0;
                color.z = 27.0 / 255.0;
            }
        } else if ( i > 2 && i < 6 ) {
            x = x3 + 2 * (i-2) * r;
            z= z3;
            if ( i == 3 ) {
                numBola = "4";
                color.x = 75.0  / 255.0;
                color.y = 21.0  / 255.0;
                color.z = 135.0 / 255.0;
            }
            if ( i == 4 ) {
                numBola = "5";
                color.x = 213.0 / 255.0;
                color.y = 105.0 / 255.0;
                color.z = 40.0  / 255.0;
            }
            if ( i == 5 ) {
                numBola = "6";
                color.x = 66.0 / 255.0;
                color.y = 105.0 / 255.0;
                color.z = 48.0 / 255.0;
            }
        } else if ( i > 5 && i < 10 ) {
            x = x4 + 2 * (i-5) * r;
            z= z4;
            if ( i == 6 ) {
                numBola = "7";
                color.x = 224.0 / 255.0;
                color.y = 24.0 / 255.0;
                color.z = 24.0 / 255.0;
            }
            if ( i == 7 ) {
                numBola = "8";
                color.x = 0.0;
                color.y = 0.0;
                color.z = 0.0;
            }
            if ( i == 8 ) {
                numBola = "9";
                color.x = 222.0 / 255.0;
                color.y = 154.0 / 255.0;
                color.z = 73.0 / 255.0;
            }
            if ( i == 9 ) {
                numBola = "10";
                color.x = 15.0 / 255.0;
                color.y = 24.0 / 255.0;
                color.z = 143.0 / 255.0;
            }
        } else if ( i > 9 && i < 15 ) {
            x = x5 + 2 * (i-9) * r;
            z= z5;
            if ( i == 10 ) {
                numBola = "11";
                color.x = 152.0 / 255.0;
                color.y = 4.0 / 255.0;
                color.z = 16.0 / 255.0;
            }
            if ( i == 11 ) {
                numBola = "12";
                color.x = 68.0 / 255.0;
                color.y = 24.0 / 255.0;
                color.z = 121.0 / 255.0;
            }
            if ( i == 12 ) {
                numBola = "13";
                color.x = 239.0 / 255.0;
                color.y = 130.0 / 255.0;
                color.z = 1.0 / 255.0;
            }
            if ( i == 13 ) {
                numBola = "14";
                color.x = 10.0 / 255.0;
                color.y = 23.0 / 255.0;
                color.z = 13.0 / 255.0;
            }
            if ( i == 14 ) {
                numBola = "15";
                color.x = 209.0 / 255.0;
                color.y = 31.0 / 255.0;
                color.z = 31.0 / 255.0;
            }
        }
        //std::cout<<"\ncentro bola "<< i<<", x= "<<x<<", y= "<<y<<", z= "<<z<<"\n";
        bola = new Bola(x, y, z, r, color.x, color.y, color.z, numBola);
        //capsa = bola->calculCapsa3D();
        //std::cout<<"centro bola "<< i <<", x= "<< capsa.pmin.x + capsa.a/2. << ", y= " << capsa.pmin.y + capsa.h/2. <<  ", z= " << capsa.pmin.z + capsa.p/2. <<"\n";
        listaConjuntBoles.push_back(bola);
    }
}

ConjuntBoles::~ConjuntBoles()
{

}

