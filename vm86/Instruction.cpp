#include "Instruction.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "InstructionMovRegReg.hpp"
#include "InstructionPop.hpp"
#include "InstructionPush.hpp"
#include "InstructionRetn.hpp"
#include "Register.hpp"

namespace vm86
{
	std::string separateSecondArg(std::string& arg)
	{
		size_t two_off = arg.find(",");
		if (two_off == std::string::npos)
		{
			throw std::runtime_error("Expected two arguments, found only 1");
		}
		std::string two = arg.substr(two_off + 1);
		if (two.find(",") != std::string::npos)
		{
			throw std::runtime_error("Expected two arguments, found at least 3");
		}
		arg.erase(two_off);
		while (two.substr(0, 1) == " ")
		{
			two.erase(0, 1);
		}
		return two;
	}

	void removeLeadingWhitespace(std::string& str)
	{
		while (str.substr(0, 1) == " ")
		{
			str.erase(0, 1);
		}
	}

	std::unique_ptr<Instruction> Instruction::fromAssembly(std::string line)
	{
		removeLeadingWhitespace(line);
		size_t comment_off = line.find(";");
		if (comment_off == std::string::npos)
		{
			const size_t last_char = line.size() - 1;
			if (line.substr(last_char, 1) == "\r")
			{
				line.erase(last_char, 1);
			}
		}
		else
		{
			line.erase(comment_off);
		}
	remove_trailing_whitespace:
		{
			const size_t last_char = line.size() - 1;
			if (line.substr(last_char, 1) == " ")
			{
				line.erase(last_char, 1);
				goto remove_trailing_whitespace;
			}
		}
		std::string ins;
		std::string arg;
		size_t arg_off = line.find(" ");
		if (arg_off == std::string::npos)
		{
			ins = std::move(line);
		}
		else
		{
			ins = line.substr(0, arg_off);
			arg = line.substr(arg_off + 1);
			removeLeadingWhitespace(arg);
			const size_t last_char = arg.size() - 1;
			if (arg.substr(last_char, 1) == "\r")
			{
				arg.erase(last_char, 1);
			}
		}
		if (ins == "mov")
		{
			std::string two = separateSecondArg(arg);
			return std::make_unique<InstructionMovRegReg>(Register::fromName(two), Register::fromName(arg));
		}
		if (ins == "push")
		{
			return std::make_unique<InstructionPush>(Register::fromName(arg).id);
		}
		if (ins == "pop")
		{
			return std::make_unique<InstructionPop>(Register::fromName(arg).id);
		}
		if (ins == "ret" || ins == "retn")
		{
			if (!arg.empty())
			{
				throw std::logic_error("Return instruction was not expected to have an argument");
			}
			return std::make_unique<InstructionRetn>();
		}
		throw std::logic_error(std::string("Unknown instruction: ").append(ins));
	}

	std::unique_ptr<Instruction> Instruction::fromBytecode(const std::vector<uint8_t>& bytecode)
	{
		auto i = bytecode.begin();
		return fromBytecode(bytecode, i);
	}

	uint8_t safelyIncrement(std::vector<uint8_t>::const_iterator& i, const std::vector<uint8_t>::const_iterator& end)
	{
		if (++i == end)
		{
			throw std::out_of_range("Unexpected end of bytecode");
		}
		return *i;
	}

#define NEXT_BYTE val = safelyIncrement(i, bytecode.end());

	std::unique_ptr<Instruction> Instruction::fromBytecode(const std::vector<uint8_t>& bytecode, std::vector<uint8_t>::const_iterator& i)
	{
		uint8_t val = *i;
		if (val == 0x48)
		{
			NEXT_BYTE;
			if (val == 0x89)
			{
				NEXT_BYTE;
				if (val & 0b11000000)
				{
					return std::make_unique<InstructionMovRegReg>(Register((RegisterId)((val >> 3) & 0b111)), Register((RegisterId)(val & 0b111)));
				}
			}
		}
		if (val == 0xC3)
		{
			return std::make_unique<InstructionRetn>();
		}
		if ((val & 0x50) == 0x50)
		{
			if ((val & 0x58) == 0x58)
			{
				return std::make_unique<InstructionPop>((RegisterId)(val & 0b111));
			}
			else
			{
				return std::make_unique<InstructionPush>((RegisterId)(val & 0b111));
			}
		}
		std::stringstream ss;
		ss << "Unknown instruction: " << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (uint16_t)val; // casting to uint16_t so clever, type-safe C++ doesn't confuse it with a char
		throw std::logic_error(ss.str());
	}

	std::vector<uint8_t> Instruction::getBytecode() const
	{
		std::vector<uint8_t> bytecode;
		getBytecode(bytecode);
		return bytecode;
	}
}
