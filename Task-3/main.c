//You do not have to change anything in this file. You will not be required to submit this file.
//Compile (from the Task-3 folder) with your Task3.c:
//   gcc main.c Task3.c -o main
//(To run the reference solution instead, swap Task3.c for Task3sol.c.)
#include "Task3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Reads one line of whitespace-separated doubles into arr (up to max values),
//returns how many values were read. Used to read matrix rows and data streams.
static int read_doubles(double* arr, int max){
    char line[600];
    if(!fgets(line, sizeof(line), stdin)) return 0;
    char* tok = strtok(line, " \t\r\n");
    int n = 0;
    while(tok && n < max){
        arr[n++] = atof(tok);
        tok = strtok(NULL, " \t\r\n");
    }
    return n;
}

//Reads one line of whitespace-separated integers into arr (up to max values),
//returns how many values were read.
static int read_ints(int* arr, int max){
    char line[600];
    if(!fgets(line, sizeof(line), stdin)) return 0;
    char* tok = strtok(line, " \t\r\n");
    int n = 0;
    while(tok && n < max){
        arr[n++] = atoi(tok);
        tok = strtok(NULL, " \t\r\n");
    }
    return n;
}

//Given helper: prints a Matrix to the terminal.
void print_matrix(Matrix* m){
    printf("Matrix (%d x %d):\n", m->rows, m->cols);
    for(int i=0;i<m->rows;i++){
        for(int j=0;j<m->cols;j++){
            double v = m->data[i][j];
            if(v == 0.0) v = 0.0; //normalise -0.00 so the output stays clean
            if(j > 0) printf(" ");
            printf("%.2f", v);
        }
        printf("\n");
    }
}

int main(){
    char line[64];
    while(1){
        printf("Task 3 Functions:\n");
        printf("1: Rolling Average\n");
        printf("2: Kalman Filter\n");
        printf("3: Kernel Convolution\n");
        printf("4: Median Filter (Bonus)\n");
        printf("0: Quit\n");
        printf("Please choose an option: ");

        if(!fgets(line, sizeof(line), stdin)) break;
        int choice = atoi(line);

        if(choice == 1){
            //--- Rolling Average ---
            printf("Please enter the number of readings: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            int n = atoi(line);

            double data[256];
            printf("Please enter the readings: ");
            int got = 0;
            while(got < n){
                got += read_doubles(data + got, n - got);
            }

            printf("Please enter the window size: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            int window = atoi(line);

            rolling_average(data, n, window);
        }
        else if(choice == 2){
            //--- Kalman Filter ---
            printf("Please enter the initial estimate: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            double est = atof(line);

            printf("Please enter the initial error covariance: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            double cov = atof(line);

            printf("Please enter the number of measurements: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            int m = atoi(line);

            double meas[256];
            printf("Please enter the measurements: ");
            int got = 0;
            while(got < m){
                got += read_doubles(meas + got, m - got);
            }

            printf("Please enter the process noise Q: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            double q = atof(line);

            printf("Please enter the measurement noise R: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            double r = atof(line);

            Kalman k;
            kalman_init(&k, est, cov);
            printf("Kalman filter output:\n");
            for(int i=0;i<m;i++){
                double out = kalman_step(&k, meas[i], q, r);
                printf("Step %d: measurement %.2f -> estimate %.2f\n", i+1, meas[i], out);
            }
        }
        else if(choice == 3){
            //--- Kernel Convolution ---
            int dims[2];
            printf("Please enter the input rows and cols: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            sscanf(line, "%d %d", &dims[0], &dims[1]);
            int rows = dims[0], cols = dims[1];

            Matrix input;
            input.rows = rows;
            input.cols = cols;
            printf("Please enter the input matrix (%d x %d):\n", rows, cols);
            for(int i=0;i<rows;i++){
                int got = 0;
                while(got < cols){
                    got += read_doubles(input.data[i] + got, cols - got);
                }
            }

            printf("Please enter the kernel size: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            int k = atoi(line);

            Matrix kernel;
            kernel.rows = k;
            kernel.cols = k;
            printf("Please enter the kernel (%d x %d):\n", k, k);
            for(int i=0;i<k;i++){
                int got = 0;
                while(got < k){
                    got += read_doubles(kernel.data[i] + got, k - got);
                }
            }

            Matrix output;
            if(convolve(&input, &kernel, &output)){
                print_matrix(&output);
            }
            else{
                printf("Convolution failed: check the kernel size.\n");
            }
        }
        else if(choice == 4){
            //--- Median Filter (Bonus) ---
            int dims[2];
            printf("Please enter the rows and cols: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            sscanf(line, "%d %d", &dims[0], &dims[1]);
            int rows = dims[0], cols = dims[1];

            Matrix input;
            input.rows = rows;
            input.cols = cols;
            printf("Please enter the input matrix (%d x %d):\n", rows, cols);
            for(int i=0;i<rows;i++){
                int got = 0;
                while(got < cols){
                    got += read_doubles(input.data[i] + got, cols - got);
                }
            }

            printf("Please enter the window size: ");
            if(!fgets(line, sizeof(line), stdin)) break;
            int window = atoi(line);

            Matrix output;
            if(median_filter(&input, &output, window)){
                print_matrix(&output);
            }
            else{
                printf("Median filter failed: check the window size.\n");
            }
        }
        else if(choice == 0){
            printf("Terminating program...\n");
            return 0;
        }
        else{
            printf("Not a valid option, please choose again.\n");
        }
    }
    return 0;
}
