#include "pch.h"
#include "Utilities.h"

namespace Psycho
{
	namespace Utilities
	{
		UINT CalculateConstantBufferAlignment(const UINT allocation)
		{
			return (allocation + 255) & ~255;
		}
	}
}