#pragma once
#include "MatrixTransposer.h"

class CpuSingleThreadTransposer : public MatrixTransposer
{
public:
	CpuSingleThreadTransposer() : MatrixTransposer("CPU Single Thread") {}

	void transpose() override;
};

