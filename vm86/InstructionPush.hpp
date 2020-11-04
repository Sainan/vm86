#pragma once

#include "Instruction.hpp"

#include "RegisterId.hpp"

namespace vm86
{
	class InstructionPush : public Instruction
	{
	private:
		RegisterId src;

	public:
		explicit InstructionPush(RegisterId src);

		[[nodiscard]] std::string getAssembly() const override;

		void getBytecode(std::vector<uint8_t>& bytecode) const override;
		[[nodiscard]] uint8_t getBytecodeLength() const override;

		void executeInVirtualMachine(VirtualMachine& vm) const override;
	};
}
