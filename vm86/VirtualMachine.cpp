#include "VirtualMachine.hpp"

#include <stdexcept>

#include "Instruction.hpp"
#include "Register.hpp"

namespace vm86
{
	VirtualMachine::VirtualMachine() noexcept
		: instruction_pointer(0), registers({ 0 })
	{
	}

	bool VirtualMachine::hasInstruction()
	{
		return getInstruction() != nullptr;
	}

	void VirtualMachine::step()
	{
		Instruction* ins = getInstruction();
		if (ins == nullptr)
		{
			throw std::range_error("Bad step call");
		}
		instruction_pointer += ins->getBytecodeLength();
		ins->executeInVirtualMachine(*this);
	}

	void VirtualMachine::run()
	{
		Instruction* ins;
		while ((ins = getInstruction()) != nullptr)
		{
			instruction_pointer += ins->getBytecodeLength();
			ins->executeInVirtualMachine(*this);
		}
	}

	void VirtualMachine::write(const Register& dst, regvalue_t val)
	{
		switch (dst.accs)
		{
		default:
			registers.at(dst.id) = val;
			break;

		case REGACCS_32:
			registers.at(dst.id) |= (val & 0xFFFFFFFF);
			break;

		case REGACCS_16:
			registers.at(dst.id) |= (val & 0xFFFF);
			break;

		case REGACCS_HIGH8:
			registers.at(dst.id) |= ((val & 0xFF) << 8);
			break;

		case REGACCS_LOW8:
			registers.at(dst.id) |= (val & 0xFF);
			break;
		}
	}

	regvalue_t VirtualMachine::read(const Register& src) const
	{
		regvalue_t val = registers.at(src.id);
		switch (src.accs)
		{
		case REGACCS_32:
			val &= 0xFFFFFFFF;
			break;

		case REGACCS_16:
			val &= 0xFFFF;
			break;

		case REGACCS_HIGH8:
			val >>= 8;
			val &= 0xFF;
			break;

		case REGACCS_LOW8:
			val &= 0xFF;
			break;
		}
		return val;
	}

	void VirtualMachine::push(regvalue_t val)
	{
		stack.emplace(val);
	}

	regvalue_t VirtualMachine::pop()
	{
		regvalue_t val = stack.top();
		stack.pop();
		return val;
	}
}
