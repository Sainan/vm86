#pragma once

#include "Instruction.hpp"

namespace vm86
{
	class InstructionRetn : public Instruction
	{
	public:
		explicit InstructionRetn();

		[[nodiscard]] std::string getAssembly() const override;

		void getBytecode(std::vector<uint8_t>& bytecode) const override;
		[[nodiscard]] uint8_t getBytecodeLength() const override;

		void executeInVirtualMachine(VirtualMachine& vm) const override;
	};
}
