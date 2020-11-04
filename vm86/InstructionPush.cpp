#include "InstructionPush.hpp"

#include "Register.hpp"

namespace vm86
{
	InstructionPush::InstructionPush(RegisterId src)
		: src(src)
	{
	}

	std::string InstructionPush::getAssembly() const
	{
		return std::string("push ").append(Register(src).getName());
	}

	void InstructionPush::getBytecode(std::vector<uint8_t>& bytecode) const
	{
		bytecode.emplace_back(0x50 | src);
	}

	uint8_t InstructionPush::getBytecodeLength() const
	{
		return 1;
	}

	void InstructionPush::executeInVirtualMachine(VirtualMachine& vm) const
	{
		// TODO
	}
}
