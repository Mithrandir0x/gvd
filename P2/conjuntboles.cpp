#include "conjuntboles.h"

ConjuntBoles::ConjuntBoles(QString tipoShading)
{
    Capsa3D capsa;
    Bola *bola;
    double x = 0.0;
    double y = 0.03075;
    double z = 0.0;
    double r = 0.03075;
    double z1 = -0.5;
    double x2 = 0.0 - 3 * r;
    double z2 = -0.5 - 2 * r * cos(30*M_PI/180.0);
    double x3 = 0.0 - 4 * r;
    double z3 = -0.5 - 4 * r * cos(30*M_PI/180.0);
    double x4 = 0.0 - 5 * r;
    double z4 = -0.5 - 6 * r * cos(30*M_PI/180.0);
    double x5 = 0.0 - 6 * r;
    double z5 = -0.5 - 8 * r * cos(30*M_PI/180.0);

    mat4 m;
    QString numBola;

    for(int i=0; i<15;i++){
        if(i==0){
            z = z1;
            numBola = "1";
        }else if(i>0 && i<3){
            x = x2 + 2 * i * r;
            z= z2;
            if(i==1)numBola = "10";
                    else numBola = "2";
        }else if(i>2 && i<6){
            x = x3 + 2 * (i-2) * r;
            z= z3;
            if(i==3)numBola = "3";
            if(i==4)numBola = "8";
            if(i==5)numBola = "11";
        }
        else if(i>5 && i<10){
            x = x4 + 2 * (i-5) * r;
            z= z4;
            if(i==6)numBola = "13";
            if(i==7)numBola = "4";
            if(i==8)numBola = "12";
            if(i==9)numBola = "5";
        }else if(i>9 && i<15){
            x = x5 + 2 * (i-9) * r;
            z= z5;
            if(i==10)numBola = "7";
            if(i==11)numBola = "14";
            if(i==12)numBola = "9";
            if(i==13)numBola = "6";
            if(i==14)numBola = "15";
        }
        //std::cout<<"\ncentro bola "<< i<<", x= "<<x<<", y= "<<y<<", z= "<<z<<"\n";
        vec4 ka = vec4(0.25, 0.20725, 0.20725, 1);
        vec4 kd = vec4(1.0, 0.829, 0.829, 1);
        vec4 ke = vec4(0.296648, 0.296648, 0.296648, 1);
        float kre = 0.088*128;

        Material *mat = new Material(ka, kd, ke, kre);
        bola = new Bola(x, y, z, r, 1.0, 1.0, 0.0, mat, tipoShading, numBola);
        //capsa = bola->calculCapsa3D();
        //std::cout<<"centro bola "<< i <<", x= "<< capsa.pmin.x + capsa.a/2. << ", y= " << capsa.pmin.y + capsa.h/2. <<  ", z= " << capsa.pmin.z + capsa.p/2. <<"\n";
        listaConjuntBoles.push_back(bola);
    }
}

ConjuntBoles::~ConjuntBoles()
{

}

