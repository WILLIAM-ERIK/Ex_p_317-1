#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 1000; // número de términos de la serie de Fibonacci
    int *fibonacci = NULL; 

    if (rank == 0) {
        fibonacci = (int*) malloc(n * sizeof(int));
        fibonacci[0] = 0;
        fibonacci[1] = 1;
    }

    int inicio = rank * (n / size) + 2; // comenzamos a partir del tercer término
    int fin = (rank + 1) * (n / size);

    if (rank == size - 1) { 
        fin = n;
    }

    int i, j;
    for (i = inicio; i < fin; i++) {
        int temp = 0;
        if (i < 2) {
            temp = i;
        } else {
            int previous = 0;
            int current = 1;
            for (j = 2; j <= i; j++) {
                temp = previous + current;
                previous = current;
                current = temp;
            }
        }

        if (rank != 0) { // enviamos el resultado al proceso 0
            MPI_Send(&temp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else {
            fibonacci[i] = temp;
            for (j = 1; j < size; j++) { // recibimos los resultados de los otros procesos
                int index = j * (n / size) + i;
                MPI_Recv(&fibonacci[index], 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    if (rank == 0) { // imprimimos la serie de Fibonacci
        printf("Serie de Fibonacci:\n");
        for (i = 0; i < n; i++) {
            printf("%d ", fibonacci[i]);
        }
        printf("\n");
        free(fibonacci);
    }

    MPI_Finalize();
    return 0;
}

