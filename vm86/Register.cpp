#include "Register.hpp"

#include <stdexcept>

#include "RegisterAccess.hpp"
#include "RegisterId.hpp"

namespace vm86
{
	Register::Register(RegisterId id, RegisterAccess accs)
		: id(id), accs(accs)
	{
		if (accs == REGACCS_HIGH8)
		{
			switch (id)
			{
			case STACK_POINTER:
			case STACK_BASE_POINTER:
			case SOURCE_INDEX:
			case DESTINATION_INDEX:
				throw std::domain_error(std::string("Can't access high 8 bits on ").append(Register(id, REGACCS_16).getName()));
			}
		}
	}

	Register Register::fromName(const std::string& _name)
	{
		std::string name = _name;
		RegisterAccess accs;
		switch (name.at(0))
		{
		case 'r':
			accs = REGACCS_64;
			name.erase(0, 1);
			break;

		case 'e':
			accs = REGACCS_32;
			name.erase(0, 1);
			break;

		default:
		{
			const size_t last_char = name.length() - 1;
			switch (name.at(last_char))
			{
			default:
				accs = REGACCS_16;
				break;

			case 'l':
				accs = REGACCS_LOW8;
				break;

			case 'h':
				accs = REGACCS_HIGH8;
				break;
			}
			name.erase(last_char, 1);
			break;
		}
		}
		if (name == "ax")
		{
			return Register(ACCUMULATOR, accs);
		}
		if (name == "cx")
		{
			return Register(COUNTER, accs);
		}
		if (name == "dx")
		{
			return Register(DATA, accs);
		}
		if (name == "bx")
		{
			return Register(BASE, accs);
		}
		if (name == "sp")
		{
			return Register(STACK_POINTER, accs);
		}
		if (name == "bp")
		{
			return Register(STACK_BASE_POINTER, accs);
		}
		if (name == "si")
		{
			return Register(SOURCE_INDEX, accs);
		}
		if (name == "di")
		{
			return Register(DESTINATION_INDEX, accs);
		}
		throw std::domain_error(std::string("Invalid register name: ").append(_name));
	}

	std::string Register::getName() const
	{
		if (accs == REGACCS_LOW8)
		{
			switch (id)
			{
			default:
				throw std::domain_error(std::string("Invalid register id: ").append(std::to_string(id)));

			case ACCUMULATOR:
				return "ah";

			case COUNTER:
				return "ch";

			case DATA:
				return "dh";

			case BASE:
				return "bh";

			case STACK_POINTER:
				return "spl";

			case STACK_BASE_POINTER:
				return "bpl";

			case SOURCE_INDEX:
				return "sil";

			case DESTINATION_INDEX:
				return "dil";
			}
		}
		if (accs == REGACCS_HIGH8)
		{
			switch (id)
			{
			default:
				throw std::domain_error(std::string("Invalid register id for REGACCS_HIGH8: ").append(std::to_string(id)));

			case ACCUMULATOR:
				return "al";

			case COUNTER:
				return "cl";

			case DATA:
				return "dl";

			case BASE:
				return "bl";
			}
		}
		const char* suffix;
		switch (id)
		{
		default:
			throw std::domain_error(std::string("Unknown or invalid register id: ").append(std::to_string(id)));

		case ACCUMULATOR:
			suffix = "ax";
			break;

		case COUNTER:
			suffix = "cx";
			break;

		case DATA:
			suffix = "dx";
			break;

		case BASE:
			suffix = "bx";
			break;

		case STACK_POINTER:
			suffix = "sp";
			break;

		case STACK_BASE_POINTER:
			suffix = "bp";
			break;

		case SOURCE_INDEX:
			suffix = "si";
			break;

		case DESTINATION_INDEX:
			suffix = "di";
			break;
		}
		if (accs == REGACCS_16)
		{
			return suffix;
		}
		if (accs == REGACCS_32)
		{
			return std::string("e").append(suffix);
		}
		if (accs == REGACCS_64)
		{
			return std::string("r").append(suffix);
		}
		throw std::domain_error(std::string("Invalid register access: ").append(std::to_string(accs)));
	}

	uint8_t Register::getBytecodeValue() const
	{
		if (accs == REGACCS_HIGH8)
		{
			switch (id)
			{
			default:
				throw std::domain_error(std::string("Can't access high 8 bits on ").append(Register(id, REGACCS_16).getName()));

			case ACCUMULATOR:
				return 0b100;

			case COUNTER:
				return 0b101;

			case DATA:
				return 0b110;

			case BASE:
				return 0b111;
			}
		}
		return id;
	}
}
