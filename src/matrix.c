#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "printing.h"
//#include "common.h"

struct matrix;
typedef struct matrix mat_t;

struct matrix {
	int i;
	int j;
	int transposed;
	double **mat_arr;
};


mat_t *matrix_create(int i, int j){
	mat_t *mat = malloc(sizeof(mat_t));

	double **mat_arr = (double **)malloc(i * sizeof(double *));  

	for (int n = 0; n < i; n++){
		mat_arr[n] = (double*)calloc(j, sizeof(double));  
	}

	mat -> i = i;
	mat -> j = j;
	mat -> transposed = 0;
	mat -> mat_arr = mat_arr;

	return mat;
}


double get_matrix_element(mat_t *matrix, int i, int j){
	return matrix -> mat_arr[i][j];
}


void matrix_print(mat_t *matrix){
	int i, j;
	double *row;
	
	i = matrix -> i;
	j = matrix -> j;
	printf("i: %d, j:%d \n-----------\n", i, j);
	int n, m;
		for (n = 0; n < i; n++){
			row = matrix -> mat_arr[n];

			for (m = 0; m < j; m++){
				printf("%f ", row[m]);
			}
			printf("\n");
		}
	printf("\n");
	return;
}


mat_t *matrix_transpose(mat_t *matrix){
	int i, j;
	double *org_row, *t_row;
	i = matrix->i;
	j = matrix->j;

	mat_t *transposed_matrix = matrix_create(j, i);
	transposed_matrix -> transposed = 1;

	int n, m;
	for (n = 0; n < i; n++){
		org_row = matrix -> mat_arr[n];
		for (m = 0; m < j; m++){
			t_row = transposed_matrix -> mat_arr[m];
			t_row[n] = org_row[m];
		}
	}
	return transposed_matrix;
}


void matrix_destroy(mat_t *matrix){
	int i;
	double *row;

	i = matrix -> i;

	
	for (int n=0; n < i; n++){
		row = matrix -> mat_arr[n];
		free(row);
	}
	free(matrix);
}


mat_t *read_matrix_from_csv(char *path){
	int MAXCHAR = 1000;

	FILE *file;
	int n_rows, n_cols;
	char row[MAXCHAR];
	char *token;

	n_rows = 0;
	n_cols = 0;
	file = fopen(path, "r");

	while (!feof(file)){
		fgets(row, MAXCHAR, file);
		n_rows++;
	}
	token = strtok(row, ",");
    while(token != NULL){
        n_cols++;
        token = strtok(NULL, ",");
    }

    mat_t *matrix = matrix_create(n_rows, n_cols);

	int n, m;
	double val;
	
	rewind(file);

	for (n=0; n < n_rows; n++){
		fgets(row, MAXCHAR, file);
		token = strtok(row, ",");
		for (m=0; m < n_cols; m++){
			val = strtod(token, NULL);

			matrix -> mat_arr[n][m] = val;
			
			token = strtok(NULL, ",");
		}
	}

	fclose(file);
	return matrix;
}


/*
 *
 *
 *BEGIN elementwise operations 
 *
 *
*/

static mat_t *elementwise_wrapper(mat_t *matrix1, mat_t *matrix2, elementwise_operator *func){
	if (matrix1 -> i != matrix2 -> i){	
		ERROR_PRINT("Dimension 0 (i) does not match for the given matrices");
	}
	else if (matrix1 -> j != matrix2 -> j){	
		ERROR_PRINT("Dimension 1 (j) does not match for the given matrices");
	}
	mat_t *res_matrix;
	int i, j;

	i = matrix1 -> i;
	j = matrix1 -> j;

	res_matrix = matrix_create(i, j);
	
	int n, m;
	double elem1, elem2;

	for (n=0; n < i; n++){
		for (m=0; m < j; m++){
			elem1 = matrix1 -> mat_arr[n][m];

			elem2 = matrix2 -> mat_arr[n][m];
			res_matrix -> mat_arr[n][m] = func(elem1, elem2);	
		}
	}
	return res_matrix;
}

static double minus(double x1, double x2){
	return x1 - x2;
}

static double addition(double x1, double x2){
	return x1 + x2;
}

static double multiplication(double x1, double x2){
	return x1 * x2;
}

static double division(double x1, double x2){
	return x1 / x2;
}

mat_t *matrix_elementwise_minus(mat_t *matrix1, mat_t *matrix2){
	return elementwise_wrapper(matrix1, matrix2, minus);
}

mat_t *matrix_elementwise_addition(mat_t *matrix1, mat_t *matrix2){
	return elementwise_wrapper(matrix1, matrix2, addition);
}

mat_t *matrix_elementwise_multiplication(mat_t *matrix1, mat_t *matrix2){
	return elementwise_wrapper(matrix1, matrix2, multiplication);
}

mat_t *matrix_elementwise_division(mat_t *matrix1, mat_t *matrix2){
	return elementwise_wrapper(matrix1, matrix2, division);
}


/*
 *
 *
 *END elementwise operations 
 *
 *
*/


mat_t *matrix_multiplication(mat_t *matrix1, mat_t *matrix2){
	if (matrix1 -> j != matrix2 -> i){	
		ERROR_PRINT("Dimension 1 (j) in matrix1 does not match the dimension 0 (i) in matrix2.");
	}
	mat_t *res_matrix;

	int i1, j1, i2, j2;

	i1 = matrix1 -> i;
	j1 = matrix1 -> j;

	i2 = matrix2 -> i;
	j2 = matrix2 -> j;

	res_matrix = matrix_create(i1, j2);
	matrix_print(matrix1);

	int n, m, s;
	double *row_m1;
	double val;
	for (n=0; n < i1; n++){
		
		row_m1 = matrix1 -> mat_arr[n];

		for (m=0; m < j2; m++){
			val = 0;
			for (s=0; s < i2; s++){
				val += row_m1[s] * matrix2 -> mat_arr[s][m];
			}	
			res_matrix -> mat_arr[n][m] = val;
		}	
	}
	return res_matrix;
}

