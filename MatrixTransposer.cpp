#include "MatrixTransposer.h"
#include <chrono>

void MatrixTransposer::copyInput(const Matrix& matrix)
{
	input = matrix;
	output = Matrix(matrix.cols, matrix.rows);
}

double MatrixTransposer::getTime()
{
	auto time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double, std::milli>(time.time_since_epoch()).count();
}
