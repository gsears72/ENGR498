#include "vectormath.h"


int main(){
    Cvector test;

    test.set_y1(120.00);
    test.set_y2(120.00);
    test.set_y3(120.00);
    Cvector temp = test.calcvector();
    test.printVector(temp);
    return 1;

}



