#pragma once

#include "fwddecl.hpp"

#include "Instruction.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace vm86
{
	class Program
	{
	public:
		std::vector<std::unique_ptr<Instruction>> instructions;

		explicit Program();

		[[nodiscard]] static Program fromAssembly(std::string str);

		[[nodiscard]] static Program fromBytecode(const std::vector<uint8_t>& bytecode);

		[[nodiscard]] std::string getAssembly() const;

		[[nodiscard]] std::vector<uint8_t> getBytecode() const;
		[[nodiscard]] size_t getBytecodeLength() const;
	};
}
