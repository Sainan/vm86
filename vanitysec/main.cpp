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

#include "../vm86/WindowsExecutableFile.hpp"

using namespace vm86;

int main(int num_args, char** args)
{
	if (num_args < 3 || num_args > 4)
	{
		std::cout << "Syntax to rename all sections: vanitysec <path> <new section names>" << std::endl;
		std::cout << "Syntax to rename select section: vanitysec <path> <target section> <new section name>" << std::endl;
	}
	else
	{
		try
		{
			WindowsExecutableFile xf(args[1]);
			const uint16_t num_sections = xf.getNumberOfSections();
			if (num_args == 3)
			{
				const std::string newname(args[2]);
				for (uint16_t i = 0; i < num_sections; i++)
				{
					std::cout << "Renaming " << xf.getSectionName(i) << std::endl;
					xf.setSectionName(i, newname);
				}
			}
			else if (num_args == 4)
			{
				const std::string oldname(args[2]);
				const std::string newname(args[3]);
				uint16_t num_sections_renamed = 0;
				for (uint16_t i = 0; i < num_sections; i++)
				{
					if (oldname == xf.getSectionName(i))
					{
						xf.setSectionName(i, newname);
						num_sections_renamed++;
					}
				}
				std::cout << num_sections_renamed << " section(s) renamed" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
