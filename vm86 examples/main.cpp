#include <iostream>

#include <windows.h>

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
#include "../vm86/WindowsExecutableFile.hpp"

using namespace vm86;

int main(int num_args, char** args)
{
	try
	{
		if (num_args > 1)
		{
			if (strcmp(args[1], "vm") == 0)
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

				return 0;
			}
			else if (num_args > 2 && strcmp(args[1], "ddd") == 0)
			{
				WindowsExecutableFile xf(args[2]);

				DataDirectoryRvaAndSize debugDataDirectoryRvaAndSize = xf.getDataDirectoryRvaAndSize(DATADIRECTORY_DEBUG);

				for (uint32_t i = 0; i < debugDataDirectoryRvaAndSize.size / sizeof(IMAGE_DEBUG_DIRECTORY); i++)
				{
					IMAGE_DEBUG_DIRECTORY debugDataDirectory = xf.readFromFileOffset<IMAGE_DEBUG_DIRECTORY>(xf.getRvaOffsetInFile(debugDataDirectoryRvaAndSize.rva) + (i * sizeof(IMAGE_DEBUG_DIRECTORY)));

					std::cout << "Debug Data Directory #" << i << std::endl;
					if (debugDataDirectory.Type == 2)
					{
						std::cout << "  Type: 2 (CODEVIEW)" << std::endl;
						std::cout << "  Path: " << xf.readString(xf.getRvaOffsetInFile(debugDataDirectory.AddressOfRawData) + 0x18) << std::endl;
					}
					else
					{
						std::cout << "  Type: " << debugDataDirectory.Type << std::endl;
					}
				}

				return 0;
			}
		}

		std::cout << "Welcome to vm86 examples." << std::endl;
		std::cout << "    I have many things to show you; you just provide the arguments!" << std::endl;
		std::cout << std::endl;
		std::cout << "vm          execute x86 bytecode in a vm and on your host machine" << std::endl;
		std::cout << "ddd <file>  read the debug data directory of the provided Windows PE file" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "There's was an exception, but according to Microsoft, exception information is not important. So, good luck with that!" << std::endl;
		return 2;
	}
	return 0;
}
