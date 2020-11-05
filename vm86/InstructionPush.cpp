#include "InstructionPush.hpp"

#include "VirtualMachine.hpp"

namespace vm86
{
	InstructionPush::InstructionPush(Register src)
		: src(src)
	{
	}

	std::string InstructionPush::getAssembly() const
	{
		return std::string("push ").append(src.getName());
	}

	void InstructionPush::getBytecode(std::vector<uint8_t>& bytecode) const
	{
		bytecode.emplace_back(0x50 | src.getBytecodeValue());
	}

	uint8_t InstructionPush::getBytecodeLength() const
	{
		return 1;
	}

	void InstructionPush::executeInVirtualMachine(VirtualMachine& vm) const
	{
		vm.push(vm.read(src));
	}
}
