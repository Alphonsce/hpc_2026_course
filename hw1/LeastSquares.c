#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int N = 100000000;
    if (argc > 1) {
        N = atoi(argv[1]);
    }

    double true_a = 2.5;
    double true_b = -1.0;
    
    double *x = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));
    
    // Generate data with Gaussian noise
    for (int i = 0; i < N; i++) {
        x[i] = (double)rand() / RAND_MAX;
        double u1 = (double)rand() / RAND_MAX;
        double u2 = (double)rand() / RAND_MAX;
        if (u1 <= 1e-9) u1 = 1e-9; // Prevent log(0)
        double noise = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2) * 0.1; // stddev = 0.1
        y[i] = true_a * x[i] + true_b + noise;
    }
    
    // Save a subset of points for plotting
    FILE *f = fopen("../results/ls_data.csv", "w");
    if (f) {
        fprintf(f, "x,y\n");
        int save_count = N > 10000 ? 10000 : N;
        for (int i = 0; i < save_count; i++) {
            fprintf(f, "%f,%f\n", x[i], y[i]);
        }
        fclose(f);
    }

    double a = 0.0;
    double b = 0.0;
    double learning_rate = 0.1;
    int epochs = 1000;

    double t1 = omp_get_wtime();

    for (int epoch = 0; epoch < epochs; epoch++) {
        double grad_a = 0.0;
        double grad_b = 0.0;

        #pragma omp parallel for reduction(+:grad_a, grad_b) schedule(static)
        for (int i = 0; i < N; i++) {
            double diff = (a * x[i] + b) - y[i];
            grad_a += diff * x[i];
            grad_b += diff;
        }

        grad_a = (grad_a * 2.0) / N;
        grad_b = (grad_b * 2.0) / N;

        a -= learning_rate * grad_a;
        b -= learning_rate * grad_b;
    }

    double t2 = omp_get_wtime();

    printf("True a: %f, True b: %f\n", true_a, true_b);
    printf("Found a: %f, Found b: %f\n", a, b);
    printf("time = %.6f\n", t2 - t1);

    FILE *f_params = fopen("../results/ls_params.csv", "w");
    if (f_params) {
        fprintf(f_params, "a,b\n");
        fprintf(f_params, "%f,%f\n", a, b);
        fclose(f_params);
    }

    free(x);
    free(y);

    return 0;
}
