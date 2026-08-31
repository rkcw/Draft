//Provided helper structures and function signatures for Task 3, please do not modify this file.
#ifndef TASK3_H
#define TASK3_H

#define MAX_DIM 16

//A fixed-capacity 2D matrix of doubles.
//matrix->data[r][c] is the value at row r, column c (both 0-based).
typedef struct{
    double data[MAX_DIM][MAX_DIM];
    int rows; //number of valid rows (1..MAX_DIM)
    int cols; //number of valid columns (1..MAX_DIM)
} Matrix;

//The state of a one-dimensional Kalman filter.
typedef struct{
    double estimate;  //current state estimate x
    double error_cov; //current error covariance P
} Kalman;

//PART A (MANDATORY)
void rolling_average(double* data, int n, int window);
void kalman_init(Kalman* k, double estimate, double error_cov);
double kalman_step(Kalman* k, double measurement, double process_noise, double measurement_noise);
int convolve(Matrix* input, Matrix* kernel, Matrix* output);

//PART B (OPTIONAL)
int median_filter(Matrix* input, Matrix* output, int window);

#endif
