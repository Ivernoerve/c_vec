#ifndef CVEC_H
#define CVEC_H


/*
 * The struct for the matrix/ vector
*/
struct matrix;
typedef struct matrix mat_t;

/*
 * Creates a new matrix with the given row and column size i, j.

 @i: The number of rows in the matrix
 @j: The number of columns in the matrix
*/


mat_t *matrix_create(int i, int j);

double get_matrix_element(mat_t *matrix, int i, int j);

void matrix_print(mat_t *matrix);


mat_t *matrix_transpose(mat_t *matrix);



mat_t *matrix_elementwise_minus(mat_t *matrix1, mat_t *matrix2);

mat_t *matrix_elementwise_addition(mat_t *matrix1, mat_t *matrix2);

mat_t *matrix_elementwise_multiplication(mat_t *matrix1, mat_t *matrix2);

mat_t *matrix_elementwise_division(mat_t *matrix1, mat_t *matrix2);

mat_t *matrix_multiplication(mat_t *matrix1, mat_t *matrix2); 

void matrix_destroy(mat_t *matrix);


mat_t *read_matrix_from_csv(char *path);

#endif
