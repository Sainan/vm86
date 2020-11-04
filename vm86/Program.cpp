#include "Program.hpp"

namespace vm86
{
	Program::Program()
		: instructions(0)
	{
	}

	Program Program::fromAssembly(std::string str)
	{
		Program p;
		size_t del_pos;
		while ((del_pos = str.find("\n")) != std::string::npos)
		{
			std::string line = str.substr(0, del_pos);
			str.erase(0, del_pos + 1);
			p.instructions.emplace_back(std::move(Instruction::fromAssembly(line)));
		}
		if (!str.empty())
		{
			p.instructions.emplace_back(std::move(Instruction::fromAssembly(str)));
		}
		return p;
	}

	Program Program::fromBytecode(const std::vector<uint8_t>& bytecode)
	{
		Program p;
		auto i = bytecode.begin();
		while (i != bytecode.end())
		{
			p.instructions.emplace_back(Instruction::fromBytecode(bytecode, i));
			i++;
		}
		return p;
	}

	std::string Program::getAssembly() const
	{
		std::string str;
		for (const auto& instruction : instructions)
		{
			str.append(instruction->getAssembly()).append("\n");
		}
		return str;
	}

	std::vector<uint8_t> Program::getBytecode() const
	{
		std::vector<uint8_t> bytecode;
		for (const auto& instruction : instructions)
		{
			instruction->getBytecode(bytecode);
		}
		return bytecode;
	}

	size_t Program::getBytecodeLength() const
	{
		size_t len = 0;
		for (const auto& instruction : instructions)
		{
			len += instruction->getBytecodeLength();
		}
		return len;
	}
}
