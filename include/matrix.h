#ifndef CVEC_H
#define CVEC_H


/*
 * The struct for the matrix/ vector
*/
struct matrix;
typedef struct matrix mat_t;


typedef double (elementwise_operator)(double, double);

typedef double (apply_operator)(double);

/*
 * Creates a new matrix with the given row and column size i, j.

 @i: The number of rows in the matrix
 @j: The number of columns in the matrix
*/


mat_t *matrix_create(int i, int j);

void matrix_destroy(mat_t *matrix);

double get_matrix_element(mat_t *matrix, int i, int j);

void set_matrix_element(mat_t *matrix, double elem, int i, int j);

int get_matrix_axis_size(mat_t *matrix, int axis);

void matrix_print(mat_t *matrix);


mat_t *matrix_transpose(mat_t *matrix);

mat_t *matrix_apply_func(mat_t *matrix, apply_operator func, int inplace);

mat_t *matrix_elementwise_minus(mat_t *matrix1, mat_t *matrix2, int inplace);

mat_t *matrix_elementwise_addition(mat_t *matrix1, mat_t *matrix2, int inplace);

mat_t *matrix_elementwise_multiplication(mat_t *matrix1, mat_t *matrix2, int inplace);

mat_t *matrix_elementwise_division(mat_t *matrix1, mat_t *matrix2, int inplace);

mat_t *matrix_multiplication(mat_t *matrix1, mat_t *matrix2); 

double matrix_sum(mat_t *matrix);

int matrix_equal(mat_t *matrix1, mat_t *matrix2, double tol);

mat_t *matrix_elementwise_equal(mat_t *matrix1, mat_t *matrix2, double tol);


mat_t *matrix_hstack(int num_matrices, ...);

mat_t *matrix_vstack(int num_matrices, ...);


mat_t *read_matrix_from_csv(char *path);

#endif
