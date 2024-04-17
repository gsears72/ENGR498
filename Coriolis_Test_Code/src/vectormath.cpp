
#include "vectormath.h"

Cvector Cvector::calcvector(){
    Cvector temp;
    fishy = alpha_fishy * (y2/y1);
    beta = alpha_beta * (y3/1);
    V = sqrt((2*y1)/q);
    temp.I = ((V*cos(beta))/cos(fishy));
    temp.J = (V*sin(beta));
    temp.K =(V*cos(beta)*tan(fishy));

    return temp;
}

void Cvector::printVector(Cvector temp){
    printf("%.7f I,%.7f J,%.7f K\n",temp.I,temp.J,temp.K);
}

void Cvector::set_y1(double input){
    y1 = input;
}

void Cvector::set_y2(double input){
    y2 = input;
}

void Cvector::set_y3(double input){
    y3 = input;
}

double Cvector::get_y1(){
    return y1;
}

double Cvector::get_y2(){
    return y2;
}

double Cvector::get_y3(){
    return y3;
}   
