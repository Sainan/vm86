#include "VirtualMachineBytecode.hpp"

namespace vm86
{
	VirtualMachineBytecode::VirtualMachineBytecode(std::vector<uint8_t>&& bytecode) noexcept
		: bytecode(std::move(bytecode))
	{
	}

	VirtualMachineBytecode::VirtualMachineBytecode(const std::vector<uint8_t>& bytecode) noexcept
		: bytecode(bytecode)
	{
	}

	VirtualMachineBytecode::VirtualMachineBytecode(VirtualMachineBytecode&& b) noexcept
		: bytecode(std::move(b.bytecode))
	{
	}

	VirtualMachineBytecode::VirtualMachineBytecode(const VirtualMachineBytecode& b) noexcept
		: bytecode(b.bytecode)
	{
	}

	Instruction* VirtualMachineBytecode::getInstruction()
	{
		if (cached_instruction_pointer != instruction_pointer)
		{
			auto i = bytecode.begin();
			i += instruction_pointer;
			cached_instruction_pointer = instruction_pointer;
			cached_instruction = std::move(Instruction::fromBytecode(bytecode, i));
		}
		return cached_instruction.get();
	}
}
