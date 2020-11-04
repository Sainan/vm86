#include <iostream>

#ifdef _DEBUG
#ifdef NOT64
#pragma comment(lib, "../vm86/Debug/vm86.lib")
#else
#pragma comment(lib, "../vm86/x64/Debug/vm86.lib")
#endif
#else
#ifdef NOT64
#pragma comment(lib, "../vm86/Release/vm86.lib")
#else
#pragma comment(lib, "../vm86/x64/Release/vm86.lib")
#endif
#endif

#include "../vm86/bytecodeToHexString.hpp"
#include "../vm86/Program.hpp"

using namespace vm86;

static int success_tally;
static int error_tally;
static int total_tally;

std::vector<uint8_t> testInstructionEndsInBytecode(const char* bytecode_hex, const char* instruction)
{
	std::vector<uint8_t> bytecode;
	try
	{
		bytecode = Program::fromAssembly(instruction).getBytecode();
		std::string gen_bytecode_hex = bytecodeToHexString(bytecode);
		if (gen_bytecode_hex == bytecode_hex)
		{
			success_tally++;
		}
		else
		{
			std::cout << instruction << " resulted in '" << gen_bytecode_hex << "', expected '" << bytecode_hex << "'" << std::endl;
			error_tally++;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		error_tally++;
	}
	total_tally++;
	return bytecode;
}

[[nodiscard]] std::string getAssemblyNoTrailingNewline(Program& p)
{
	std::string str = p.getAssembly();
	str.erase(str.length() - 1, 1);
	return str;
}

void testInstruction(const char* bytecode_hex, const char* instruction)
{
	try
	{
		auto bytecode = testInstructionEndsInBytecode(bytecode_hex, instruction);
		// Test bytecode ends in instruction (or at least the bytecode from Program.getBytecode())
		Program p = Program::fromBytecode(bytecode);
		std::string str = getAssemblyNoTrailingNewline(p);
		if (str == instruction)
		{
			success_tally++;
		}
		else
		{
			std::cout << bytecodeToHexString(bytecode) << " resulted in '" << str << "', expected '" << instruction << "'" << std::endl;
			error_tally++;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		error_tally++;
	}
	total_tally++;
}

int main()
{
	// Init:
	success_tally = 0;
	error_tally = 0;
	total_tally = 0;

	// Test against truth:
	testInstruction("C3", "retn");
	testInstruction("56", "push rsi");
	testInstruction("57", "push rdi");
	testInstruction("55", "push rbp");
	testInstruction("53", "push rbx");
	testInstruction("5B", "pop rbx");
	testInstruction("5D", "pop rbp");
	testInstruction("5F", "pop rdi");
	testInstruction("5E", "pop rsi");
	testInstruction("48 89 C8", "mov rax, rcx");
	testInstruction("48 89 C1", "mov rcx, rax");
	testInstruction("48 89 CA", "mov rdx, rcx");
	testInstruction("48 89 D1", "mov rcx, rdx");

	// Test aliases:
	testInstructionEndsInBytecode("C3", "ret");

	// Test spacing:
	testInstructionEndsInBytecode("56", "push     rsi");

	// Test comments:
	testInstructionEndsInBytecode("48 89 C8", "mov rax, rcx ; copying rax from rcx");

	// Results:
	std::cout << total_tally << " assertions: " << success_tally << " succeeded, " << error_tally << " failed" << std::endl;

	return error_tally;
}
