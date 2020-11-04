#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace vm86
{
	[[nodiscard]] extern std::string bytecodeToHexString(std::vector<uint8_t>& bytecode);
}
