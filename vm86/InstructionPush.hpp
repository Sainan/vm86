#pragma once

#include "Instruction.hpp"

#include "Register.hpp"

namespace vm86
{
	class InstructionPush : public Instruction
	{
	private:
		Register src;

	public:
		explicit InstructionPush(Register src);

		[[nodiscard]] std::string getAssembly() const override;

		void getBytecode(std::vector<uint8_t>& bytecode) const override;
		[[nodiscard]] uint8_t getBytecodeLength() const override;

		void executeInVirtualMachine(VirtualMachine& vm) const override;
	};
}
