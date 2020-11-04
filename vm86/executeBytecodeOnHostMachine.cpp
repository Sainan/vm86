#include "executeBytecodeOnHostMachine.hpp"

#include <stdexcept>

#include <windows.h>

namespace vm86
{
	regvalue_t executeBytecodeOnHostMachine(const std::vector<uint8_t>& bytecode, regvalue_t reg_counter, regvalue_t reg_data)
	{
		void* const buffer = VirtualAlloc(nullptr, bytecode.size(), MEM_COMMIT, PAGE_READWRITE);
		if (buffer == nullptr)
		{
			throw std::runtime_error("VirtualAlloc failed");
		}
		std::memcpy(buffer, bytecode.data(), bytecode.size());
		DWORD old_protect;
		VirtualProtect(buffer, bytecode.size(), PAGE_EXECUTE_READ, &old_protect);
		const regvalue_t rax = ((regvalue_t(__fastcall*)(regvalue_t, regvalue_t))buffer)(reg_counter, reg_data);
		VirtualFree(buffer, 0, MEM_RELEASE);
		return rax;
	}
}
