#include "InstructionPop.hpp"

#include "Register.hpp"

namespace vm86
{
	InstructionPop::InstructionPop(RegisterId dst)
		: dst(dst)
	{
	}

	std::string InstructionPop::getAssembly() const
	{
		return std::string("pop ").append(Register(dst).getName());
	}

	void InstructionPop::getBytecode(std::vector<uint8_t>& bytecode) const
	{
		bytecode.emplace_back(0x58 | dst);
	}

	uint8_t InstructionPop::getBytecodeLength() const
	{
		return 1;
	}

	void InstructionPop::executeInVirtualMachine(VirtualMachine& vm) const
	{
		// TODO
	}
}
