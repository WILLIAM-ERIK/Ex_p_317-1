#include <stdio.h>
#include <omp.h>

int main()
{
    int i, num = 1000;
    double x, sum = 0.0, step = 1.0 / (double) num;

    #pragma omp parallel for private(x) reduction(+:sum)
    for (i = 0; i < num; i++)
    {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x*x);
    }

    double pi = step * sum;
    printf("Valor de pi: %f\n", pi);

    return 0;
}

