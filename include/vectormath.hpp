#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <vector>
#include <string>
#include <USB/USBAPI.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "CsvData.h"

class Cvector{
    public:
        // Output vector
        double I = 0.0;
        double J = 0.0;
        double K = 0.0;

        //DP input vector
        double y1 = 0.0;
        double y2 = 1.0;
        double y3 = 0.0;

        // staic values
        double density = 1.225;
        std::string lookupTable = "lookup.csv";

        //Values for calculations
        double q = 0.00;
        double V = 0.00;
        double V_0 = 0.00;
        double V_A = 0.00;
        double y = 0.00;
        double temp_Ca = 0.00;
        double temp_Cb = 0.00;
        std::vector<double> S;
        std::vector<double> p;
        double gamma_a = 0.00;
        double gamma_b = 0.00;
        double Ca = 0.00;
        double Cb = 0.00;
        double angle = 0.00;
        double slope = 1.0;
        double intercept = 1.0;

        void calcvector();

        void printVector();

        void set_y1(double input);
        void set_y2(double input);
        void set_y3(double input);
		void setYs(double* inY1, double* inY2, double* inY3);

        void set_V_A(double input);

		double lookupAngleFromTable();

        double dotProduct(const std::vector<double>& vec1, const std::vector<double>& vec2);

        std::vector<double> scalarMultiplication(const std::vector<double>& vec, double scalar);

        void leastSquares(const std::vector<double>& x, const std::vector<double>& y, double& slope, double& intercept);

        std::vector<double> vectorMultiplication(const std::vector<double>& vec1, const std::vector<double>& vec2);

		std::tuple<double,double,double> startup_calibration_sensor_reading();

};

#endif