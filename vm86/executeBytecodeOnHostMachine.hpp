#pragma once

#include "fwddecl.hpp"

#include <vector>

namespace vm86
{
	// Only works on Windows! Feel free to adjust .cpp with a preprocessor `if` so that the code works for your OS.
	regvalue_t executeBytecodeOnHostMachine(const std::vector<uint8_t>& bytecode, regvalue_t reg_counter = 0, regvalue_t reg_data = 0);
}
