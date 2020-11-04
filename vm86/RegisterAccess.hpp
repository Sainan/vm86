#pragma once

#include <cstdint>

namespace vm86
{
	enum RegisterAccess : uint8_t
	{
		REGACCS_64,
		REGACCS_32,
		REGACCS_16,
		REGACCS_HIGH8,
		REGACCS_LOW8,
	};
}
