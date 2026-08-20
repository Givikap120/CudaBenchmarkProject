#pragma once

#include <string>
#include "Matrix.h"

class MatrixTransposer
{
protected:
	Matrix input, output;
	std::string name;

public:
	MatrixTransposer(const std::string& name) : name(name) {}

	virtual ~MatrixTransposer() = default;

	virtual void copyInput(const Matrix& matrix);
	virtual void transpose() = 0;

	virtual void startTimer() {}
	virtual void stopTimer() {}
	virtual void synchronize() {}

	const std::string& getName() const { return name; }
	const Matrix& getResult() const { return output; }
};

