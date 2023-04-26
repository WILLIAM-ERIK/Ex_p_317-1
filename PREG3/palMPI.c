#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size, i, n;
    char word[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Ingrese una palabra: ");
        scanf("%s", word);
        n = strlen(word);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int start = rank * (n / size);
    int end = (rank + 1) * (n / size);
    if (rank == size - 1)
        end = n;

    int palindrome = 1;
    for (i = start; i < end / 2; i++)
    {
        if (word[i] != word[n - 1 - i])
        {
            palindrome = 0;
            break;
        }
    }

    int global_palindrome;
    MPI_Reduce(&palindrome, &global_palindrome, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        if (global_palindrome)
            printf("La palabra %s VERDADERO - es un palindromo.\n", word);
        else
            printf("La palabra %s FALSO - no es un palindromo.\n", word);
    }

    MPI_Finalize();
    return 0;
}

