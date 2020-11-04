#include "VirtualMachineStandard.hpp"

#include "Program.hpp"

namespace vm86
{
	VirtualMachineStandard::VirtualMachineStandard() noexcept
		: VirtualMachine(), Program()
	{
	}

	void VirtualMachineStandard::resetInstructions()
	{
		instructions.clear();
		instruction_pointer = 0;
	}

	void VirtualMachineStandard::loadInstructions(Program&& p)
	{
		resetInstructions();
		for (auto&& instruction : p.instructions)
		{
			instructions.emplace_back(std::move(instruction));
		}
	}

	void VirtualMachineStandard::loadInstructions(const std::string& str)
	{
		loadInstructions(Program::fromAssembly(str));
	}

	Instruction* VirtualMachineStandard::getInstruction()
	{
		regvalue_t ip = 0;
		for (const auto& instruction : instructions)
		{
			if (ip == instruction_pointer)
			{
				return instruction.get();
			}
			ip += instruction->getBytecodeLength();
		}
		return nullptr;
	}
}
