#include "vectormath.h"
#include <USB/USBAPI.h>


Cvector Cvector::calcvector(Cvector calc){
    
    // lefte square =
    // correct y1 = lefe square thing
    // C_beta = y3/corrected y1
    Serial.println();
    Serial.print("alpha_fishy: ");
    Serial.println(calc.alpha_fishy);
    Serial.print("y2: ");
    Serial.println(calc.y2);
    Serial.print("y1: ");
    Serial.println(calc.y1);

    calc.fishy = calc.alpha_fishy * (calc.y2/calc.y1);
    Serial.println();
    Serial.print("fishy: ");
    Serial.println(calc.fishy);
    Serial.println();

    Serial.print("alpha_fishy: ");
    Serial.println(calc.alpha_fishy);
    Serial.print("y3: ");
    Serial.println(calc.y3);
    Serial.println();

    calc.beta = calc.alpha_beta * (calc.y3/1);
    Serial.print("beta: ");
    Serial.println(calc.beta);
    Serial.println();

    Serial.print("y1: ");
    Serial.println(calc.y1);
    Serial.print("q: ");
    Serial.println(calc.q);
    Serial.println();

    calc.V = sqrt((2*calc.y1)/calc.q);
    Serial.print("V: ");
    Serial.println(calc.V);
    Serial.println();

    calc.I = ((calc.V*cos(calc.beta))/cos(calc.fishy));
    Serial.print("I: ");
    Serial.println(calc.I);
    Serial.println();

    calc.J = (calc.V*sin(calc.beta));
    Serial.print("J: ");
    Serial.println(calc.J);
    Serial.println();

    calc.K =(calc.V*cos(calc.beta)*tan(calc.fishy));
    Serial.print("K: ");
    Serial.println(calc.K);
    Serial.println();

    return calc;
}

void Cvector::printVector(Cvector temp){
    printf("%f I,%f J,%f K",temp.I,temp.J,temp.K);
}

void Cvector::set_y1(double input){
    Serial.println("bp12");
    y1 = abs(input);
    Serial.println(y1);
}

void Cvector::set_y2(double input){
    Serial.println("bp13");
    y2 = abs(input);
    Serial.println(y2);
}

void Cvector::set_y3(double input){
    Serial.println("bp14");
    y3 = abs(input);
    Serial.println(y3);
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




