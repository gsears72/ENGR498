#ifndef VECTORMATH_H
#define VECTORMATH_H


#include <math.h>
#include <cstdio>


class Cvector{
    public:
        double I = 0.0;
        double J = 0.0;
        double K = 0.0;
        double y1 = 0.0;
        double y2 = 0.0;
        double y3 = 0.0;
        double q = 1.00;
        double V = 1.00;
        double V_hat = 1.00;
        double fishy = 1.00;
        double beta = 1.00;
        double alpha_fishy = 1.00;
        double alpha_beta = 1.00;

        Cvector calcvector();

        void printVector(Cvector temp);

        void set_y1(double input);

        void set_y2(double input);

        void set_y3(double input);

};

#endif