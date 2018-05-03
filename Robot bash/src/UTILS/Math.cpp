#include "stdafx.h"
#include "Math.h"

#include <math.h>

namespace Math
{
	float RoundToN(float val, int n)
	{
		int multiplier = (int)(val / n);

		if (fmod(val, (float)n) < (n *0.5f))
		{
			return (float)multiplier * n;
		}
		else
		{
			return (float)(multiplier + 1) * n;
		}
	}
}
