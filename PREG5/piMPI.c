#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N_TOTAL 1000

double calcular_pi(int ini, int fin) {
    double suma = 0.0;
    for (int i = ini; i < fin; i++) {
        if (i % 2 == 0) {
            suma += 1.0 / (2 * i + 1);
        } else {
            suma -= 1.0 / (2 * i + 1);
        }
    }
    return suma;
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int n = N_TOTAL / world_size;
    int start = world_rank * n;
    int end = start + n;

    double local_sum = calcular_pi(start, end);
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        double pi = 4.0 * global_sum;
        printf("pi = %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}

