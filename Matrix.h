#pragma once

#include <vector>

struct Matrix {
	size_t rows;
	size_t cols;
	std::vector<float> data;

	Matrix(int r, int c) : rows(r), cols(c), data(r* c)
	{	}

	float& operator()(size_t r, size_t c) {
		return data[r * cols + c];
	}

	float operator()(size_t r, size_t c) const {
		return data[r * cols + c];
	}
};
