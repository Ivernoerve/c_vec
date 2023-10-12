#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "printing.h"

#include <math.h>

struct matrix;
typedef struct matrix mat_t;

struct matrix {
	int i;
	int j;
	mat_t *transposed;
	double **mat_arr;
	int has_changed;
};


mat_t *matrix_create(int i, int j){
	mat_t *mat = malloc(sizeof(mat_t));

	double **mat_arr = (double **)malloc(i * sizeof(double *));  

	for (int n = 0; n < i; n++){
		mat_arr[n] = (double*)calloc(j, sizeof(double));  
	}

	mat -> i = i;
	mat -> j = j;
	mat -> transposed = NULL;
	mat -> mat_arr = mat_arr;
	mat -> has_changed = 0;

	return mat;
}


void matrix_destroy(mat_t *matrix){
	int i;
	double *row;

	i = matrix -> i;

	for (int n=0; n < i; n++){
		row = matrix -> mat_arr[n];
		free(row);
	}
	if (matrix -> transposed != NULL)
		matrix_destroy(matrix -> transposed);
	free(matrix);
}


double get_matrix_element(mat_t *matrix, int i, int j){
	return matrix -> mat_arr[i][j];
}


void set_matrix_element(mat_t *matrix, double elem, int i, int j){
	if (matrix -> i <= i)
		ERROR_PRINT("Value i: %d is out of range for matrix with size i: %d", i, matrix -> i);
	else if (matrix -> j <= j)
		ERROR_PRINT("Value j: %d is out of range for matrix with size j: %d", j, matrix -> j);

	matrix -> mat_arr[i][j] = elem;
}


int get_matrix_axis_size(mat_t *matrix, int axis){
	if (axis == 0)
		return matrix -> i;
	else if (axis == 1)
		return matrix -> j;

	ERROR_PRINT("Axis %d is out of bound, spessify axis 0 for dim i, axis 1 for dim j", axis);
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

	if (matrix -> transposed != NULL && matrix -> has_changed == 0)
		return matrix -> transposed;

	int i, j;
	double *org_row, *t_row;
	i = matrix->i;
	j = matrix->j;
	mat_t *transposed_matrix;

	if (matrix -> transposed == NULL)
		transposed_matrix = matrix_create(j, i);
	else
		transposed_matrix = matrix -> transposed;

	int n, m;
	for (n = 0; n < i; n++){
		org_row = matrix -> mat_arr[n];
		for (m = 0; m < j; m++){
			t_row = transposed_matrix -> mat_arr[m];
			t_row[n] = org_row[m];
		}
	}
	matrix -> transposed = transposed_matrix;
	matrix -> has_changed = 0;
	return transposed_matrix;
}


mat_t *matrix_apply_func(mat_t *matrix, apply_operator func, int inplace){
	double **arr_ptr;
	int i, j;
	mat_t *res_matrix;

	res_matrix = NULL;
	i = matrix -> i;
	j = matrix -> j;
	if (inplace == 1){
		arr_ptr = matrix -> mat_arr;
	}
	else if (inplace == 0){
		
		res_matrix = matrix_create(i, j);
		arr_ptr = res_matrix -> mat_arr;
	}
	else{
		ERROR_PRINT("Arg {inplace} should be boolean 1, 0 for true false respectively.");
	}

	int n, m;
	for (n=0; n < i; n++){
		for (m=0; m < j; m++){

			arr_ptr[n][m] = func(matrix -> mat_arr[n][m]);
		}
	}

	res_matrix -> has_changed = 1;
	return res_matrix;
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

static mat_t *elementwise_wrapper(mat_t *matrix1, mat_t *matrix2, elementwise_operator *func, int inplace){
	if (matrix1 -> i != matrix2 -> i){	
		ERROR_PRINT("Dimension 0 (i) does not match for the given matrices %d != %d", matrix1->i, matrix2->i);
	}
	else if (matrix1 -> j != matrix2 -> j){	
		ERROR_PRINT("Dimension 1 (j) does not match for the given matrices %d != %d", matrix1->j, matrix2->j);
	}
	mat_t *res_matrix;
	int i, j;
	double **arr_ptr;

	res_matrix = NULL;
	i = matrix1 -> i;
	j = matrix1 -> j;

	if (inplace == 1){
		res_matrix =  matrix1;
	}
	else if (inplace == 0){
		res_matrix = matrix_create(i, j);
	}
	else{
		ERROR_PRINT("Arg {inplace} should be boolean 1, 0 for true false respectively.");
	}

	arr_ptr = res_matrix -> mat_arr;
	
	int n, m;
	double elem1, elem2;

	for (n=0; n < i; n++){
		for (m=0; m < j; m++){
			elem1 = matrix1 -> mat_arr[n][m];

			elem2 = matrix2 -> mat_arr[n][m];
			arr_ptr[n][m] = func(elem1, elem2);
		}
	}
	res_matrix -> has_changed = 1;
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

mat_t *matrix_elementwise_minus(mat_t *matrix1, mat_t *matrix2, int inplace){
	return elementwise_wrapper(matrix1, matrix2, minus, inplace);
}

mat_t *matrix_elementwise_addition(mat_t *matrix1, mat_t *matrix2, int inplace){
	return elementwise_wrapper(matrix1, matrix2, addition, inplace);
}

mat_t *matrix_elementwise_multiplication(mat_t *matrix1, mat_t *matrix2, int inplace){
	return elementwise_wrapper(matrix1, matrix2, multiplication, inplace);
}

mat_t *matrix_elementwise_division(mat_t *matrix1, mat_t *matrix2, int inplace){
	return elementwise_wrapper(matrix1, matrix2, division, inplace);
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
		ERROR_PRINT("Dimension 1 (j) in matrix1 does not match the dimension 0 (i) in matrix2 %d != %d", matrix1->j, matrix2->i);
	}
	mat_t *res_matrix;

	int i1, i2, j2;

	i1 = matrix1 -> i;


	i2 = matrix2 -> i;
	j2 = matrix2 -> j;

	res_matrix = matrix_create(i1, j2);


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
	res_matrix -> has_changed = 1;
	return res_matrix;
}


double matrix_sum(mat_t *matrix){
	double sum;
	int i, j;

	i = matrix -> i;
	j = matrix -> j;
	sum = 0;

	int n, m;

	for (n=0; n < i; n++){
		for (m=0; m < j; m++){
			sum += fabs(matrix -> mat_arr[n][m]);
		}
	}
	return sum;
}


/*
 *
 *
 *BEGIN logical operators
 *
 *
*/


mat_t *matrix_elementwise_equal(mat_t *matrix1, mat_t *matrix2, double tol){
	if (matrix1 -> i != matrix2 -> i){	
		ERROR_PRINT("Dimension 0 (i) does not match for the given matrices %d != %d", matrix1->i, matrix2->i);
	}
	else if (matrix1 -> j != matrix2 -> j){	
		ERROR_PRINT("Dimension 1 (j) does not match for the given matrices %d != %d", matrix1->j, matrix2->j);
	}
	mat_t *res_matrix;
	int i, j;

	i = matrix1 -> i;
	j = matrix1 -> j;

	res_matrix = matrix_create(i, j);

	int n, m;
	double val1, val2;
	for (n=0; n < i; n++){
		for (m=0; m < j; m++){

			val1 = matrix1 -> mat_arr[n][m];
			val2 = matrix2 -> mat_arr[n][m];

			if (val2 < val1 + tol && val2 > val1 - tol)
				res_matrix -> mat_arr[n][m] = 1;
		}
	}
	return res_matrix;
}


int *matrix_equal(mat_t *matrix1, mat_t *matrix2, double tol){
	if (matrix1 -> i != matrix2 -> i){	
		ERROR_PRINT("Dimension 0 (i) does not match for the given matrices %d != %d", matrix1->i, matrix2->i);
	}
	else if (matrix1 -> j != matrix2 -> j){	
		ERROR_PRINT("Dimension 1 (j) does not match for the given matrices %d != %d", matrix1->j, matrix2->j);
	}

	int i, j;

	i = matrix1 -> i;
	j = matrix1 -> j;

	int n, m;
	double val1, val2;
	for (n=0; n < i; n++){
		for (m=0; m < j; m++){
			val1 = matrix1 -> mat_arr[n][m];
			val2 = matrix2 -> mat_arr[n][m];
				if (val2 > val1 + tol && val2 < val1 - tol)
					return 0;
		}
	}
	return 1;
}


/*
 *
 *
 *END logical operators
 *
 *
*/

