#include "InstructionRetn.hpp"

namespace vm86
{
	InstructionRetn::InstructionRetn()
	{
	}

	std::string InstructionRetn::getAssembly() const
	{
		return "retn";
	}

	void InstructionRetn::getBytecode(std::vector<uint8_t>& bytecode) const
	{
		bytecode.emplace_back(0xC3);
	}

	uint8_t InstructionRetn::getBytecodeLength() const
	{
		return 1;
	}

	void InstructionRetn::executeInVirtualMachine(VirtualMachine& vm) const
	{
		// TODO
	}
}
