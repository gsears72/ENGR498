#include "vectormath.hpp"

void Cvector::calcvector(){
    this->temp_Ca = this->y2/this->y1;
    this->temp_Cb = this->y3/this->y1;

    this->V_0 = (2*this->y1)/this->density;
    //this.V_A is set every 100 samples

    this->y = this->V_A - this->V_0;

    double s1 = this->V_0*this->temp_Ca;
    double s2 = this->V_0*this->temp_Cb;
    this->S ={s1,s2};

    //NEED TO FIGURE OUT HOW TO DO LEAST SQUARES FIT
    std::vector<double> p_part1 = vectorMultiplication(this->S,this->S);
    std::vector<double> p_part2 = scalarMultiplication(this->S,this->y);
     // least squares fit
    leastSquares(p_part1,p_part2,this->slope,this->intercept);

    this->gamma_a = this->slope;
    this->gamma_b = this->intercept;

    this->V = this->V_0*(1 +(this->gamma_a*this->temp_Ca) + (this->gamma_b*this->temp_Cb));

    double partial_q = ((1)
    +(this->gamma_a*pow(this->temp_Ca,2))
    +(this->gamma_b*pow(this->temp_Cb,2)));
    this->q = this->y1* pow(partial_q,2);

    this->Ca = this->y2/this->q;
    this->Cb = this->y/this->q;

    //vector look-up
    this->angle = lookupAngleFromTable();
    // Convert angle to radians
    double angleRadians = this->angle * M_PI / 180.0;

    // calculate I,J,K vectors
    //NEEDS TO BE REWORKED
    this->I = (this->V*cos(angleRadians));

    this->J = (this->V*sin(angleRadians));

    this->K =(this->V*tan(angleRadians));
}

void Cvector::printVector(){
	char printBuffer[50];
    sprintf(printBuffer, "%f I,%f J,%f K",this->I,this->J,this->K);
	Serial.println(printBuffer);
	Serial.flush();
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

void Cvector::setYs(double* inY1, double* inY2, double* inY3) {
	this->set_y1(*inY1);
	this->set_y2(*inY2);
	this->set_y3(*inY3);
}

void Cvector::set_V_A(double input){ 
    V_A = abs(input);
}

double Cvector::lookupAngleFromTable() {
	// Search for the row with matching C_alpha, C_beta, and V
    for (const auto& row : csvData) {
        if (sizeof(row) >= 5) { // Assuming C_alpha, C_beta, V, and Angle are in columns 1, 2, 3, and 4 respectively
            double alpha = row[0];
            double beta = row[1];
            double v = row[2];
            if (alpha == this->Ca && beta == this->Cb && v == this->V) {
                return row[3]; // Return the angle
            }
        }
    }

    // If no matching row is found
    std::cerr << "Error: No matching data found for C_alpha=" << this->Ca << ", C_beta=" << this->Cb << ", V=" << this->V << std::endl;
    return 0.0; // or some appropriate error handling
}

//Vector multiplication

// Function to calculate dot product of two 1x2 vectors
double Cvector::dotProduct(const std::vector<double>& vec1, const std::vector<double>& vec2) {
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
std::vector<double> Cvector::scalarMultiplication(const std::vector<double>& vec, double scalar) {
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
void Cvector::leastSquares(const std::vector<double>& x, const std::vector<double>& y, double& slope, double& intercept) {
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
std::vector<double> Cvector::vectorMultiplication(const std::vector<double>& vec1, const std::vector<double>& vec2) {
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





