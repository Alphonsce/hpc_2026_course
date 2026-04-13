#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

float dotprod(float *a, float *b, size_t N)
{
    int i;
    float sum = 0.0f;

#pragma omp parallel for reduction(+ : sum)
    for (i = 0; i < (int)N; ++i)
    {
        sum += a[i] * b[i];
        printf("tid = %d i = %d\n", omp_get_thread_num(), i);
    }

    return sum;
}

int main(int argc, char *argv[])
{
    const size_t N = 100;
    int i;
    float sum;
    float a[N], b[N];

    for (i = 0; i < (int)N; ++i)
    {
        a[i] = b[i] = (float)i;
    }

    sum = dotprod(a, b, N);

    printf("Sum = %f\n", sum);

    return 0;
}
