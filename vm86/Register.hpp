#pragma once

#include <cstdint>
#include <string>

#include "RegisterId.hpp"
#include "RegisterAccess.hpp"

namespace vm86
{
	class Register
	{
	public:
		RegisterId id;
		RegisterAccess accs;

		explicit Register(RegisterId id, RegisterAccess accs = REGACCS_64);

		[[nodiscard]] static Register fromName(const std::string& name);

		[[nodiscard]] std::string getName() const;

		[[nodiscard]] uint8_t getBytecodeValue() const;

		[[nodiscard]] uint8_t getAccessBits() const;
		void assertAccessBitsAreSameAs(Register& b);
	};
}
