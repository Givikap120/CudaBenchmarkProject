#include "MatrixTransposer.h"
#include <chrono>

void MatrixTransposer::copyInput(const Matrix& matrix)
{
	input = matrix;
	output = Matrix(matrix.cols, matrix.rows);
}
