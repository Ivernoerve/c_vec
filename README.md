# c_vec
2d Vector library in C to allow for basic matrix operations. 

The library implements a matrix struct containing the dimensions and a array containing the values itself. 

Support for creating a marrix from csv is implemented.


## Supported math operations:
--
* Matrix transposing
* All four elementary elementwise operations. 
	* Addition
	* Subtraction
	* Multiplication
	* Division
* Matrix multiplication


## TODO 
--

Create a set rule for whether operations should be inplace or should return new matrix structs.

Improve structuring of the matrix transpose to keep track of the transposes of matrices in memory.

Implement more operations such as total_sum, row_sum and col_sum.

Implement a wrapper function that allows for alteration of elements in a matrix with operations such as exponentials.