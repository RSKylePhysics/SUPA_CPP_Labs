#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <cmath>  
#include <vector>

void LSQ(const std::vector<float>& x, const std::vector<float>& y, float& p, float& q) {
    int N = x.size(); // This allows us to define the length of the array for the loop
    float tx= 0, ty = 0, txy = 0, txx = 0; 

    for (int i = 0; i < N; ++i) { // This loop sums the data in totalX, totalY, total X*Y and total X*X arrays
        tx += x[i];
        ty += y[i];
        txy += x[i] * y[i];
        txx += x[i] * x[i];
    }
    p = (N * txy - tx * ty) / (N * txx - tx * tx); //Equation for p
    q = (ty - p * tx) / N; // Equation for q
}

float CST(const std::vector<float>& x, const std::vector<float>& y, float p, float q, const std::vector<float>& s) {
    float csq = 0.0; //chi-squared 
    int N = x.size(); 
    for (int i = 0; i < N; ++i) {
        float exp= p * x[i] + q; // expected yval
        csq += std::pow((y[i] - exp) / s[i], 2);
    }
    return csq;
} 

float Magnitude(float x, float y);
int main() {
    std::ifstream file("input2D_float.txt");
    std::string line;
    float x, y;
    char comma;
    std::getline(file,line);
    std::vector<float> x_vals, y_vals;
    std::ofstream Q1OUT("Q1_output.txt"); //save file for task 1
    std::ofstream Q2OUT("Q2_output.txt"); //save file for task 2
    std::ofstream Q3OUT("Q3_output.txt"); //save file for task 3
    while (std::getline(file,line)) {
        std::istringstream iss(line);
        iss >> x >> comma >> y;
        float mag = Magnitude(x,y);
        std::cout << "x:" << x << ", y: " << y << std::endl; //print x and y
        std::cout <<"Magnitude:" << mag << std::endl; //calculate magnitude assuming init pos of (0,0)

        x_vals.push_back(x); // This weird command stores the data I am using a loop to read
        y_vals.push_back(y); // This is so much easier in python

        Q1OUT << "x:" << x << ", y: " << y << std::endl; // saving task 1 as file
        Q2OUT << "x:" << x << ", y: " << y << std::endl; // saving task 2 as file
        Q2OUT <<"Magnitude:" << mag << std::endl; // still saving task 2

    }
    Q1OUT.close();
    Q2OUT.close();
    float p, q;
    LSQ(x_vals, y_vals, p, q);
    std::cout << "Best fit: y = " << p << "x + " << q << std::endl;
    float EVal;
    std::ifstream err_file("error2D_float.txt");
    std::string err_line;
    std::getline(err_file,err_line);
    std::vector<float> xerr, yerr;
    while (err_file >> EVal) {
        std::istringstream iss_error(err_line);
        iss_error >> x >> comma >> y;
        xerr.push_back(x);
        yerr.push_back(y);
    }
    // Calculate chi-squared 
    float csqy = CST(x_vals, y_vals, p, q, yerr);
    float csqx = CST(x_vals, y_vals, p, q, xerr);
    std::cout << "X-Chi-squared: " << csqx << std::endl;
    std::cout << "Y-Chi-squared: " << csqy << std::endl;
    Q3OUT <<"X-Chi-squared::" << csqx << std::endl; // saving task 3 
    Q3OUT <<"Y-Chi-squared::" << csqy << std::endl; // saving task 3 
    
    return 0;
}
float Magnitude(float x, float y) {
    return sqrt(x*x + y*y);
}