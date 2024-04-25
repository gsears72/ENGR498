#include "vectormath.h"
#include <USB/USBAPI.h>
#include <fstream>
#include <sstream>
#include <iostream>


Cvector Cvector::calcvector(Cvector calc){
    calc.temp_Ca = calc.y2/calc.y1;
    calc.temp_Cb = calc.y3/calc.y1;

    calc.V_0 = (2*calc.y1)/calc.density;
    //calc.V_A is set every 100 samples

    calc.y = calc.V_A - calc.V_0;

    double s1 = calc.V_0*calc.temp_Ca;
    double s2 = calc.V_0*calc.temp_Cb;
    calc.S ={s1,s2};

    //NEED TO FIGURE OUT HOW TO DO LEAST SQUARES FIT
    std::vector<double >p_part1 = vectorMultiplication(calc.S,calc.S);
    std::vector<double> p_part2 = scalarMultiplication(calc.S,calc.y);
     // least squares fit
    leastSquares(p_part1,p_part2,calc.slope,calc.intercept);

    calc.gamma_a = calc.slope;
    calc.gamma_b = calc.intercept;

    calc.V = calc.V_0*(1 +(calc.gamma_a*calc.temp_Ca) + (calc.gamma_b*calc. temp_Cb));

    double partial_q = ((1)
    +(calc.gamma_a*pow(calc.temp_Ca,2))
    +(calc.gamma_b*pow(calc.temp_Cb,2)));
    calc.q = calc.y1* pow(partial_q,2);

    calc.Ca = calc.y2/calc.q;
    calc.Cb = calc.y/calc.q;

    //vector look-up
    calc.angle = lookupAngleFromCSV(calc.lookupTable, calc.Ca, calc.Cb, calc.V);
    // Convert angle to radians
    double angleRadians = calc.angle * M_PI / 180.0;

    // calculate I,J,K vectors
    //NEEDS TO BE REWORKED
    calc.I = (calc.V*cos(angleRadians));
    Serial.print("I: ");
    Serial.println(calc.I);
    Serial.println();

    calc.J = (calc.V*sin(angleRadians));
    Serial.print("J: ");
    Serial.println(calc.J);
    Serial.println();

    calc.K =(calc.V*tan(angleRadians));
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
    y2 = abs(input);
}

void Cvector::set_y3(double input){
    y3 = abs(input);
}

void Cvector::set_V_A(double input){ 
    V_A = abs(input);
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

// Function to parse CSV file
std::vector<std::vector<std::string>> parseCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to lookup angle from CSV file
double lookupAngleFromCSV(const std::string& filename, double C_alpha, double C_beta, double V) {
    std::vector<std::vector<std::string>> csvData;
    try {
        csvData = parseCSV(filename);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0.0; // or some appropriate error handling
    }

    // Search for the row with matching C_alpha, C_beta, and V
    for (const auto& row : csvData) {
        if (row.size() >= 5) { // Assuming C_alpha, C_beta, V, and Angle are in columns 1, 2, 3, and 4 respectively
            double alpha = std::stod(row[0]);
            double beta = std::stod(row[1]);
            double v = std::stod(row[2]);
            if (alpha == C_alpha && beta == C_beta && v == V) {
                return std::stod(row[3]); // Return the angle
            }
        }
    }

    // If no matching row is found
    std::cerr << "Error: No matching data found for C_alpha=" << C_alpha << ", C_beta=" << C_beta << ", V=" << V << std::endl;
    return 0.0; // or some appropriate error handling
}

//Vector multiplication

// Function to calculate dot product of two 1x2 vectors
double dotProduct(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    // Check if vectors are of the same size
    if (vec1.size() != 2 || vec2.size() != 2) {
        std::cerr << "Error: Vectors must be of size 2 for dot product calculation" << std::endl;
        return 0.0;
    }

    // Perform dot product calculation
    double result = vec1[0] * vec2[0] + vec1[1] * vec2[1];

    return result;
}

// Function to perform scalar multiplication of a 1x2 vector
std::vector<double> scalarMultiplication(const std::vector<double>& vec, double scalar) {
    // Check if the vector is of the correct size (1x2)
    if (vec.size() != 2) {
        std::cerr << "Error: Input vector must be of size 2 for scalar multiplication" << std::endl;
        return {};
    }

    // Perform scalar multiplication
    std::vector<double> result(2);
    result[0] = vec[0] * scalar;
    result[1] = vec[1] * scalar;

    return result;
}

// Function to perform least squares regression between a double and a vector of doubles
void leastSquares(const std::vector<double>& x, const std::vector<double>& y, double& slope, double& intercept) {
    int n = x.size();
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x_sq = 0;

    // Calculate sums
    for (int i = 0; i < n; ++i) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x_sq += x[i] * x[i];
    }

    // Calculate slope (m) and intercept (b)
    slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x_sq - sum_x * sum_x);
    intercept = (sum_y - slope * sum_x) / n;
}

// Function to perform element-wise multiplication of two vectors
std::vector<double> vectorMultiplication(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    // Check if vectors are of the same size
    if (vec1.size() != vec2.size()) {
        std::cerr << "Error: Vectors must be of the same size for element-wise multiplication" << std::endl;
        return std::vector<double>();
    }

    // Perform element-wise multiplication
    std::vector<double> result;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result.push_back(vec1[i] * vec2[i]);
    }

    return result;
}





