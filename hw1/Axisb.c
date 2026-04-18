#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int N = 2000;
    int max_iters = 500;
    double tolerance = 1e-6;

    if (argc > 1) {
        N = atoi(argv[1]);
    }

    double *A = (double *)malloc(N * N * sizeof(double));
    double *b = (double *)malloc(N * sizeof(double));
    double *x = (double *)calloc(N, sizeof(double));
    double *x_new = (double *)malloc(N * sizeof(double));

    // Initialize with a diagonally dominant matrix to ensure convergence
    for (int i = 0; i < N; i++) {
        double row_sum = 0.0;
        for (int j = 0; j < N; j++) {
            double val = (double)(rand() % 100) / 100.0;
            A[i * N + j] = val;
            row_sum += val;
        }
        A[i * N + i] += row_sum + 1.0; 
        b[i] = (double)(rand() % 100) / 100.0;
    }

    double t1 = omp_get_wtime();

    int iter;
    for (iter = 0; iter < max_iters; iter++) {
        
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    sum += A[i * N + j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / A[i * N + i];
        }

        double max_diff = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = fabs(x_new[i] - x[i]);
            if (diff > max_diff) {
                max_diff = diff;
            }
            x[i] = x_new[i];
        }

        if (max_diff < tolerance) {
            break;
        }
    }

    double t2 = omp_get_wtime();

    printf("Converged in %d iterations\n", iter);
    printf("time = %.6f\n", t2 - t1);

    free(A);
    free(b);
    free(x);
    free(x_new);

    return 0;
}
