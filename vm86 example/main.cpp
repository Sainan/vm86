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

#include "../vm86/executeBytecodeOnHostMachine.hpp"
#include "../vm86/Register.hpp"
#include "../vm86/VirtualMachineStandard.hpp"

using namespace vm86;

int main()
{
	VirtualMachineStandard vm;

	vm.write(Register(COUNTER), 1);

	std::cout << "rax: " << vm.read(Register(ACCUMULATOR)) << std::endl;
	std::cout << "rcx: " << vm.read(Register(COUNTER)) << std::endl;

	std::cout << "--- executing ---" << std::endl;
	vm.loadInstructions("mov rax, rcx\nretn");
	vm.run();

	std::cout << "rax: " << vm.read(Register(ACCUMULATOR)) << std::endl;
	std::cout << "rcx: " << vm.read(Register(COUNTER)) << std::endl;

	std::cout << "--- executing on host ---" << std::endl;

	std::cout << "rax: " << executeBytecodeOnHostMachine(vm.getBytecode(), 1);
}
