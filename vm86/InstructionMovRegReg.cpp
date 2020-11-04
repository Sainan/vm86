#include "InstructionMovRegReg.hpp"

#include "VirtualMachine.hpp"

namespace vm86
{
	InstructionMovRegReg::InstructionMovRegReg(Register src, Register dst)
		: src(src), dst(dst)
	{
	}

	std::string InstructionMovRegReg::getAssembly() const
	{
		return std::string("mov ").append(dst.getName()).append(", ").append(src.getName());
	}

	void InstructionMovRegReg::getBytecode(std::vector<uint8_t>& bytecode) const
	{
		bytecode.emplace_back(0x48);
		bytecode.emplace_back(0x89);
		bytecode.emplace_back(0b11000000 | (src.getBytecodeValue() << 3) | dst.getBytecodeValue());
	}

	uint8_t InstructionMovRegReg::getBytecodeLength() const
	{
		return 3;
	}

	void InstructionMovRegReg::executeInVirtualMachine(VirtualMachine& vm) const
	{
		vm.write(dst, vm.read(src));
	}
}
