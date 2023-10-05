#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"


struct matrix;
typedef struct matrix mat_t;

struct matrix {
	int i;
	float j;
	double **mat_arr;
};


mat_t *matrix_create(int i, int j){
	mat_t *mat = malloc(sizeof(mat_t));

	double **mat_arr = (double **)malloc(i * sizeof(double *));  

	for (int n = 0; n < i; n++){
		mat_arr[n] (double*)calloc(j * sizeof(double));  
	}

	mat -> i = i;
	mat -> j = j;
	mat -> mat_arr = mat_arr;

	return mat;
}


void matrix_print(mat_t *matrix){
	int i, j;
	double *row;

	i = matrix -> i;
	j = matrix -> j;

	for (int n = 0; nn < i; n++){
		row = matrix -> mat_arr[n];

		for (int m; m < j; m++){
			printf("%d ", row[m]);
		}
		printf("\n");
	}

}


