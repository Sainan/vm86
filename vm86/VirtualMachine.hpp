#pragma once

#include "fwddecl.hpp"

#include <array>
#include <cstdint>
#include <stack>

#include "RegisterId.hpp"

namespace vm86
{
	class VirtualMachine
	{
	public:
		regvalue_t instruction_pointer;

		std::array<regvalue_t, NUM_REGISTERS> registers;

		std::stack<regvalue_t> stack;

		explicit VirtualMachine() noexcept;

		[[nodiscard]] virtual Instruction* getInstruction() = 0; // Gets the instruction that the instruction pointer points to
		[[nodiscard]] bool hasInstruction();
		void step();
		void run();

		void write(const Register& dst, regvalue_t val);
		[[nodiscard]] regvalue_t read(const Register& src) const;

		void push(regvalue_t val);
		regvalue_t pop();
	};
}
