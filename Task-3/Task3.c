//You are NOT ALLOWED to add any other libraries
//
#include "Task3.h"

#include <stdio.h>
//

//You are allowed to define helpers in this file to assist you in implementing the required functions. However, you are NOT allowed to modify the function signatures of the provided functions in Task3.h.

//PART A (MANDATORY)
void rolling_average(double* data, int n, int window){
    printf("Rolling average called.\n");
}

void kalman_init(Kalman* k, double estimate, double error_cov){
    //TODO for the student.
}

double kalman_step(Kalman* k, double measurement, double process_noise, double measurement_noise){
    //TODO for the student.
    printf("Kalman step called.\n");
    return 0.0;
}

int convolve(Matrix* input, Matrix* kernel, Matrix* output){
    //TODO for the student.
    printf("Convolve called.\n");
    return 0;
}

//PART B (OPTIONAL)
int median_filter(Matrix* input, Matrix* output, int window){
    //You can leave this one blank if you do not want to implement it.
    printf("Median filter called.\n");
    return 0;
}
