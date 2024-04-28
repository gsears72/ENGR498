#include "vectormath.h"
#include <USB/USBAPI.h>


Cvector Cvector::calcvector(Cvector calc){

    calc.fishy = calc.alpha_fishy * (calc.y2/calc.y1);

    calc.beta = calc.alpha_beta * (calc.y3/1);
    
    calc.V = sqrt((2*calc.y1)/calc.q);

    calc.I = ((calc.V*cos(calc.beta))/cos(calc.fishy));
    Serial.print(calc.I);
    Serial.print(",");


    calc.J = (calc.V*sin(calc.beta));
    Serial.print(calc.J);
    Serial.print(",");

    calc.K =(calc.V*cos(calc.beta)*tan(calc.fishy));
    Serial.println(calc.K);


    return calc;
}

void Cvector::printVector(Cvector temp){
    printf("%f I,%f J,%f K",temp.I,temp.J,temp.K);
}

void Cvector::set_y1(double input){
    y1 = abs(input);
}

void Cvector::set_y2(double input){
    y2 = abs(input);
}

void Cvector::set_y3(double input){
    y3 = abs(input);
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




