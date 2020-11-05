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
#include "../vm86/executeBytecodeOnHostMachine.hpp"
#include "../vm86/Program.hpp"
#include "../vm86/Register.hpp"
#include "../vm86/VirtualMachineStandard.hpp"

using namespace vm86;

// Static vars
static int success_tally;
static int error_tally;

// Util functions
[[nodiscard]] std::string getAssemblyNoTrailingNewline(Program& p)
{
	std::string str = p.getAssembly();
	str.erase(str.length() - 1, 1);
	return str;
}

// Assert functions
std::vector<uint8_t> testAssemblyEndsInBytecode(const char* const bytecode_hex, const char* const assembly)
{
	std::vector<uint8_t> bytecode;
	try
	{
		bytecode = Program::fromAssembly(assembly).getBytecode();
		std::string gen_bytecode_hex = bytecodeToHexString(bytecode);
		if (gen_bytecode_hex == bytecode_hex)
		{
			success_tally++;
		}
		else
		{
			std::cout << "'" << assembly << "' resulted in '" << gen_bytecode_hex << "', expected '" << bytecode_hex << "'" << std::endl;
			error_tally++;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		error_tally++;
	}
	return bytecode;
}

void testBytecodeEndsInAssembly(const std::vector<uint8_t>& bytecode, const char* const assembly)
{
	try
	{
		Program p = Program::fromBytecode(bytecode);
		std::string str = getAssemblyNoTrailingNewline(p);
		if (str == assembly)
		{
			success_tally++;
		}
		else
		{
			std::cout << "'" << bytecodeToHexString(bytecode) << "' resulted in '" << str << "', expected '" << assembly << "'" << std::endl;
			error_tally++;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		error_tally++;
	}
}

void testAssemblyBytecodeParity(const char* const bytecode_hex, const char* const assembly)
{
	testBytecodeEndsInAssembly(testAssemblyEndsInBytecode(bytecode_hex, assembly), assembly);
}

void testRax(const char* const assembly, const regvalue_t expected_rax, const regvalue_t rcx = 0, const regvalue_t rdx = 0)
{
	VirtualMachineStandard vm;
	vm.write(Register(COUNTER), rcx);
	vm.write(Register(DATA), rdx);
	vm.loadInstructions(assembly);
	vm.run();
	{
		const regvalue_t rax = vm.read(Register(ACCUMULATOR));
		if (rax == expected_rax)
		{
			success_tally++;
		}
		else
		{
			error_tally++;
			std::cout << "'" << assembly << "' in VM with (" << rcx << ", " << rdx << ") resulted in " << rax << ", expected " << expected_rax << std::endl;
		}
	}
	const auto bytecode = vm.getBytecode();
	{
		const regvalue_t rax = executeBytecodeOnHostMachine(bytecode, rcx, rdx);
		if (rax == expected_rax)
		{
			success_tally++;
		}
		else
		{
			error_tally++;
			std::cout << "'" << assembly << "' on host with (" << rcx << ", " << rdx << ") resulted in " << rax << ", expected " << expected_rax << std::endl;
		}
	}
	testBytecodeEndsInAssembly(bytecode, assembly);
}

int main()
{
	// Init:
	success_tally = 0;
	error_tally = 0;

	// Test against truth:
	testAssemblyBytecodeParity("C3", "retn");
	testAssemblyBytecodeParity("56", "push rsi");
	testAssemblyBytecodeParity("57", "push rdi");
	testAssemblyBytecodeParity("55", "push rbp");
	testAssemblyBytecodeParity("53", "push rbx");
	testAssemblyBytecodeParity("5B", "pop rbx");
	testAssemblyBytecodeParity("5D", "pop rbp");
	testAssemblyBytecodeParity("5F", "pop rdi");
	testAssemblyBytecodeParity("5E", "pop rsi");
	testAssemblyBytecodeParity("48 89 C8", "mov rax, rcx");
	testAssemblyBytecodeParity("48 89 C1", "mov rcx, rax");
	testAssemblyBytecodeParity("48 89 CA", "mov rdx, rcx");
	testAssemblyBytecodeParity("48 89 D1", "mov rcx, rdx");

	// Test rax, also against host:
	testRax("mov rax, rcx\nretn", 0x1337CAFEBABE1337, 0x1337CAFEBABE1337, 0);
	testRax("mov rax, rdx\nretn", 0x1337CAFEBABE1337, 0, 0x1337CAFEBABE1337);
	testRax("push rcx\npop rax\nretn", 0x1337CAFEBABE1337, 0x1337CAFEBABE1337, 0);
	testRax("push rdx\npop rax\nretn", 0x1337CAFEBABE1337, 0, 0x1337CAFEBABE1337);

	// Test aliases:
	testAssemblyEndsInBytecode("C3", "ret");

	// Test spacing:
	testAssemblyEndsInBytecode("56", "    push     rsi    ");

	// Test comments:
	testAssemblyEndsInBytecode("48 89 C8", "mov rax, rcx ; copying rax from rcx");

	// Results:
	std::cout << (success_tally + error_tally) << " assertions: " << success_tally << " succeeded, " << error_tally << " failed" << std::endl;

	return error_tally;
}
