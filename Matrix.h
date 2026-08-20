#pragma once

#include <vector>

struct Matrix {
	size_t rows;
	size_t cols;
	std::vector<float> data;

	Matrix(int r, int c) : rows(r), cols(c), data(r* c)
	{}

	Matrix() : rows(0), cols(0)
	{}

	float& operator()(size_t r, size_t c) {
		return data[r * cols + c];
	}

	float operator()(size_t r, size_t c) const {
		return data[r * cols + c];
	}

	bool operator==(const Matrix& other) const {
		if (rows != other.rows || cols != other.cols)
			return false;

		for (size_t i = 0; i < rows * cols; ++i) {
			if (data[i] != other.data[i])
				return false;
		}

		return true;
	}

	bool operator!=(const Matrix& other) const {
		return !(*this == other);
	}
};
