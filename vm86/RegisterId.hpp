#pragma once

#include <cstdint>

namespace vm86
{
	enum RegisterId : uint8_t
	{
		ACCUMULATOR = 0b000,
		COUNTER = 0b001,
		DATA = 0b010,
		BASE = 0b011,
		STACK_POINTER = 0b100,
		STACK_BASE_POINTER = 0b101,
		SOURCE_INDEX = 0b110,
		DESTINATION_INDEX = 0b111,

		NUM_REGISTERS
	};
}
