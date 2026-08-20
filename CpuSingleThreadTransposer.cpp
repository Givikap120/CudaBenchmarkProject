#include "CpuSingleThreadTransposer.h"

void CpuSingleThreadTransposer::transpose()
{
	for (size_t r = 0; r < input.rows; ++r)
	{
		for (size_t c = 0; c < input.cols; ++c)
		{
			output(c, r) = input(r, c);
		}
	}
}
