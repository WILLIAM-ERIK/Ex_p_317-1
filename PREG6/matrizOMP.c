#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

//Calcula el producto de matriz xB, pero solo de la fila indicada por pos
void obtenerFila(int pos,int A[][100],int x[][100],int B[][100],int ntam)
{
	int j,n;
	for (j=0;j< ntam; j=j+1){
		A[pos][j]=0;
		for (n=0;n< ntam; n=n+1){
			A[pos][j]=A[pos][j]+(x[pos][n]*(B[n][j]));
		}			  
	}
}
void imprimirMatriz(int mat[][100],int ntam)
{	int i,j;
	for (i=0;i< ntam; i=i+1){
		for(j=0;j< ntam; j=j+1){
		  printf("%d  \t",mat[i][j]);
		}
		printf("\n");
	}
}
int main ()
{
	int i,j;

	int A[100][100];
	int x[100][100];
	int B[100][100];

	int ntam=100;
	for (i=0;i< ntam; i=i+1){
		for(j=0;j< ntam; j=j+1){
			x[i][j]=rand()%10;
			B[i][j]=rand()%10;
		}
	}

	//printf("\n Matriz x \n");
	//imprimirMatriz(x,ntam);

	//printf("\n Matriz B \n");
	//imprimirMatriz(B,ntam);


	time_t inicio,fin;
	time(&inicio);

	#pragma omp parallel
	{
		//En cada proceso paralelo se ejecuta obtenerFila() para una fila i
		for (i=0;i< ntam; i=i+1){
			obtenerFila(i,A,x,B,ntam);
		}
	}

	printf("\n Matriz A=xB \n");
	imprimirMatriz(A,ntam);

	time(&fin);     
	printf("\n Tiempo: %.2f segundos",difftime(fin,inicio)); 
	return 0;
}	  
