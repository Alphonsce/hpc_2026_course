#include <omp.h>
#include <stdio.h>

int main()
{
    const size_t N = 1000000;
    double step, t1, t2;

    double pi, sum = 0.;

    step = 1. / (double)N;
    t1 = omp_get_wtime();

    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < N; ++i)
    {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1. + x * x);
    }
    t2 = omp_get_wtime();

    pi = step * sum;

    printf("pi = %.16f\n", pi);
    printf("time = %.6f\n", t2 - t1);

    return 0;
}
