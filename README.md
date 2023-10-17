# c_vec
2d Vector library in C to allow for basic matrix operations. 

The library implements a matrix struct containing the dimensions and a array containing the values itself. 

Support for creating a marrix from csv is implemented.


## Supported math operations:

* Matrix transposing
* All four elementary elementwise operations. 
	* Addition
	* Subtraction
	* Multiplication
	* Division
* Matrix multiplication
* Sum


## Supported Logical operations:

* Global equality test
	* Tests global equality between two matrices returns true, false
* Elementwise equality
	* Tests equality elementwise returns matrix with 1, 0 for equal, not equal elements. 

## Supported restructuring operations

* vstacking
	* Stacking two matrices along the 0th dimension (rows)
* hstacking
	* Stacking two matrices along the 1st dimension (collumns)

## TODO 


Create a set rule for whether operations should be inplace or should return new matrix structs.

Improve structuring of the matrix transpose to keep track of the transposes of matrices in memory.

Implement more operations such as total_sum, row_sum and col_sum.

