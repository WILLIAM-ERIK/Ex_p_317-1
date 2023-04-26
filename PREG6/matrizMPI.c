#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 100
#define COLS 100

int main(int argc, char** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int i, j, k;

    int matrixA[ROWS][COLS];
    int matrixB[ROWS][COLS];
    int matrixC[ROWS][COLS];

    if (rank == 0) {
        // Inicializar matrices A y B
        for (i = 0; i < ROWS; i++) {
            for (j = 0; j < COLS; j++) {
                matrixA[i][j] = rand() % 100;
                matrixB[i][j] = rand() % 100;
            }
        }
    }

    // Dividir filas de la matriz A entre procesos
    int rows_per_process = ROWS / size;
    int rows_to_send[rows_per_process][COLS];
    MPI_Scatter(matrixA, rows_per_process * COLS, MPI_INT, rows_to_send, rows_per_process * COLS, MPI_INT, 0, MPI_COMM_WORLD);

    // Enviar matriz B a todos los procesos
    MPI_Bcast(matrixB, ROWS * COLS, MPI_INT, 0, MPI_COMM_WORLD);

    // Multiplicar las filas recibidas de la matriz A por la matriz B
    int local_result[rows_per_process][COLS];
    for (i = 0; i < rows_per_process; i++) {
        for (j = 0; j < COLS; j++) {
            local_result[i][j] = 0;
            for (k = 0; k < ROWS; k++) {
                local_result[i][j] += rows_to_send[i][k] * matrixB[k][j];
            }
        }
    }

    // Recopilar los resultados parciales de cada proceso en la matriz C
    MPI_Gather(local_result, rows_per_process * COLS, MPI_INT, matrixC, rows_per_process * COLS, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Imprimir la matriz resultante C
        printf("Matriz resultante:\n");
        for (i = 0; i < ROWS; i++) {
            for (j = 0; j < COLS; j++) {
                printf("%d ", matrixC[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}

