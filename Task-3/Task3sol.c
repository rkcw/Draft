//Reference solution for Task 3.
//Compile with: gcc main.c Task3sol.c -o main
#include "Task3.h"

#include <stdio.h>

//PART A (MANDATORY)
void rolling_average(double* data, int n, int window){
    printf("Rolling average (window %d):\n", window);
    printf("%-6s%-10s%s\n", "Index", "Reading", "Average");
    for(int i=0;i<n;i++){
        double sum = 0.0;
        int start = i - window + 1;
        if(start < 0){
            start = 0;
        }
        int count = i - start + 1;
        for(int j=start;j<=i;j++){
            sum += data[j];
        }
        printf("%-6d%-10.2f%.2f\n", i, data[i], sum/count);
    }
}

void kalman_init(Kalman* k, double estimate, double error_cov){
    k->estimate = estimate;
    k->error_cov = error_cov;
}

double kalman_step(Kalman* k, double measurement, double process_noise, double measurement_noise){
    //Prediction: the state itself is static, so only the covariance grows.
    double p_pred = k->error_cov + process_noise;
    //Update: compute the Kalman gain, then fuse the measurement in.
    double gain = p_pred / (p_pred + measurement_noise);
    k->estimate = k->estimate + gain * (measurement - k->estimate);
    k->error_cov = (1.0 - gain) * p_pred;
    return k->estimate;
}

int convolve(Matrix* input, Matrix* kernel, Matrix* output){
    int k = kernel->rows;
    //The kernel must be square, of odd size, and small enough to fit.
    if(k <= 0 || (k % 2) == 0){
        return 0;
    }
    if(k > input->rows || k > input->cols){
        return 0;
    }
    int orows = input->rows - k + 1;
    int ocols = input->cols - k + 1;
    for(int r=0;r<orows;r++){
        for(int c=0;c<ocols;c++){
            double sum = 0.0;
            for(int i=0;i<k;i++){
                for(int j=0;j<k;j++){
                    sum += input->data[r+i][c+j] * kernel->data[i][j];
                }
            }
            output->data[r][c] = sum;
        }
    }
    output->rows = orows;
    output->cols = ocols;
    return 1;
}

//PART B (OPTIONAL)
int median_filter(Matrix* input, Matrix* output, int window){
    if(window <= 0 || (window % 2) == 0){
        return 0;
    }
    int half = window / 2;
    output->rows = input->rows;
    output->cols = input->cols;
    for(int r=0;r<input->rows;r++){
        for(int c=0;c<input->cols;c++){
            double vals[MAX_DIM * MAX_DIM];
            int count = 0;
            for(int i=-half;i<=half;i++){
                for(int j=-half;j<=half;j++){
                    int rr = r + i;
                    int cc = c + j;
                    double v = 0.0; //out-of-bounds neighbours are treated as 0
                    if(rr >= 0 && rr < input->rows && cc >= 0 && cc < input->cols){
                        v = input->data[rr][cc];
                    }
                    vals[count++] = v;
                }
            }
            //Bubble sort the neighbourhood ascending, then take the middle value.
            for(int a=0;a<count-1;a++){
                for(int b=a+1;b<count;b++){
                    if(vals[a] > vals[b]){
                        double t = vals[a];
                        vals[a] = vals[b];
                        vals[b] = t;
                    }
                }
            }
            output->data[r][c] = vals[count/2];
        }
    }
    return 1;
}
