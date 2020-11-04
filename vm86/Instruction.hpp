#pragma once

#include "fwddecl.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace vm86
{
	class Instruction
	{
	public:
		[[nodiscard]] static std::unique_ptr<Instruction> fromAssembly(std::string line);

		[[nodiscard]] static std::unique_ptr<Instruction> fromBytecode(const std::vector<uint8_t>& bytecode);
		[[nodiscard]] static std::unique_ptr<Instruction> fromBytecode(const std::vector<uint8_t>& bytecode, std::vector<uint8_t>::const_iterator& i);

		[[nodiscard]] virtual std::string getAssembly() const = 0;

		virtual void getBytecode(std::vector<uint8_t>& bytecode) const = 0;
		[[nodiscard]] std::vector<uint8_t> getBytecode() const;
		[[nodiscard]] virtual uint8_t getBytecodeLength() const = 0;

		virtual void executeInVirtualMachine(VirtualMachine& vm) const = 0;
	};
}
