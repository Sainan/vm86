#pragma once

#include "Instruction.hpp"

#include "Register.hpp"

namespace vm86
{
	class InstructionPop : public Instruction
	{
	private:
		Register dst;

	public:
		explicit InstructionPop(Register dst);

		[[nodiscard]] std::string getAssembly() const override;


		void getBytecode(std::vector<uint8_t>& bytecode) const override;
		[[nodiscard]] uint8_t getBytecodeLength() const override;

		void executeInVirtualMachine(VirtualMachine& vm) const override;
	};
}
