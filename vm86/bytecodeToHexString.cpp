#include "bytecodeToHexString.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace vm86
{
	std::string bytecodeToHexString(const std::vector<uint8_t>& bytecode)
	{
		std::stringstream ss;
		auto i = bytecode.begin();
		if (i != bytecode.end())
		{
			while (true)
			{
				ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (uint16_t)*i; // Yeah, uint8_t is char to stringstream... thanks type-saftey
				i++;
				if (i == bytecode.end())
				{
					break;
				}
				ss << " ";
			}
		}
		return ss.str();
	}
}
