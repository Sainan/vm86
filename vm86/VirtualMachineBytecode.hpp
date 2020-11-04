#pragma once

#include "VirtualMachine.hpp"

#include <memory>
#include <vector>

#include "Instruction.hpp"

namespace vm86
{
	class VirtualMachineBytecode : public VirtualMachine
	{
	private:
		regvalue_t cached_instruction_pointer = 0;
		std::unique_ptr<Instruction> cached_instruction{};

	public:
		std::vector<uint8_t> bytecode;

		explicit VirtualMachineBytecode(std::vector<uint8_t>&& bytecode) noexcept;
		explicit VirtualMachineBytecode(const std::vector<uint8_t>& bytecode) noexcept;
		explicit VirtualMachineBytecode(VirtualMachineBytecode&& b) noexcept;
		explicit VirtualMachineBytecode(const VirtualMachineBytecode& b) noexcept;

		[[nodiscard]] Instruction* getInstruction() final;
	};
}
