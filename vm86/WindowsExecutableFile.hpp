#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace vm86
{
	// A class for reading and writing Windows executable files (.exe & .dll)

	// Credits:
	// - https://en.wikibooks.org/wiki/X86_Disassembly/Windows_Executable_Files
	// - https://www.sweetscape.com/010editor/repository/files/EXE.bt

	class WindowsExecutableFile
	{
	public:
		std::fstream file;
		uint32_t pe_header_offset = 0;

		explicit WindowsExecutableFile(std::fstream&& file);
		explicit WindowsExecutableFile(const std::string& path);

		template <typename T>
		T readFromFileOffset(std::streamoff off)
		{
			file.seekg(off);
			T val{};
			file.read((char*)&val, sizeof(T));
			return val;
		}

		// === COFF Header ===
		[[nodiscard]] uint16_t getCompiledForMachine(); // 0x14c for i386, 0x8664 for x86_64, anything else for "oh no"
		[[nodiscard]] uint16_t getNumberOfSections();
		[[nodiscard]] uint32_t getTimestamp();
		[[nodiscard]] uint32_t getNumberOfSymbols();
		[[nodiscard]] uint16_t getSizeOfOptionalHeader();
		[[nodiscard]] uint16_t getCharacteristics();

		// === Sections ===
		[[nodiscard]] std::streamoff getSectionOffsetInFile(uint16_t section);
		[[nodiscard]] std::string getSectionName(uint16_t section);
		void setSectionName(uint16_t section, const std::string& name); // UTF-8, max. 8 bytes
		[[nodiscard]] uint32_t getSectionVirtualAddress(uint16_t section);
		[[nodiscard]] uint32_t getSectionDataSize(uint16_t section);
		[[nodiscard]] uint32_t getSectionDataOffsetInFile(uint16_t section);
		[[nodiscard]] std::vector<uint8_t> getSectionData(uint16_t section);
	};
}
