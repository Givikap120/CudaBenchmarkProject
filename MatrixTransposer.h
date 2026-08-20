#pragma once

#include <string>
#include "Matrix.h"

class MatrixTransposer
{
	Matrix input, output;
	std::string name;

public:
	MatrixTransposer(const std::string& name) : name(name) {}

	virtual ~MatrixTransposer() = default;

	virtual void copyInput(const Matrix& matrix);
	virtual void transpose() = 0;
	virtual double getTime();

	const std::string& getName() const { return name; }
	const Matrix& getResult() const { return output; }
};

