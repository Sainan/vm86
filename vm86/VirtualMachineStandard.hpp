#pragma once

#include "VirtualMachine.hpp"
#include "Program.hpp"

#include <memory>
#include <vector>

#include "Instruction.hpp"

namespace vm86
{
	class VirtualMachineStandard : public VirtualMachine, public Program
	{
	public:
		explicit VirtualMachineStandard() noexcept;

		void resetInstructions();
		void loadInstructions(Program&& p);
		void loadInstructions(const std::string& str);

		[[nodiscard]] Instruction* getInstruction() final;
	};
}
