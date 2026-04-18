#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    const size_t N = 100;
    const size_t chunk = 3;

    int i;
    float a[N], b[N], c[N];

    for (i = 0; i < (int)N; ++i)
    {
        a[i] = b[i] = (float)i;
    }

#pragma omp parallel for schedule(static, chunk) shared(a, b, c)
    for (i = 0; i < (int)N; ++i)
    {
        c[i] = a[i] + b[i];
        printf("tid = %d, c[%d] = %f\n", omp_get_thread_num(), i, c[i]);
    }

    return 0;
}
