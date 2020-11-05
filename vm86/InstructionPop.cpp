#include "InstructionPop.hpp"

#include "VirtualMachine.hpp"

namespace vm86
{
	InstructionPop::InstructionPop(Register dst)
		: dst(dst)
	{
	}

	std::string InstructionPop::getAssembly() const
	{
		return std::string("pop ").append(dst.getName());
	}

	void InstructionPop::getBytecode(std::vector<uint8_t>& bytecode) const
	{
		bytecode.emplace_back(0x58 | dst.getBytecodeValue());
	}

	uint8_t InstructionPop::getBytecodeLength() const
	{
		return 1;
	}

	void InstructionPop::executeInVirtualMachine(VirtualMachine& vm) const
	{
		vm.write(dst, vm.pop());
	}
}
