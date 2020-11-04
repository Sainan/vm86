#include "WindowsExecutableFile.hpp"

#include <stdexcept>

namespace vm86
{
	WindowsExecutableFile::WindowsExecutableFile(std::fstream&& _file)
		: file(std::move(_file))
	{
		if (!file.is_open())
		{
			throw std::runtime_error("fstream is not open");
		}
		const uint16_t mz = readFromFileOffset<uint16_t>(0);
		if (mz != ('M' | ('Z' << 8)))
		{
			throw std::domain_error(std::string("Expected file to start with 'MZ', found '").append(std::string((char*)&mz, 2)).append("'"));
		}
		pe_header_offset = readFromFileOffset<uint32_t>(0x3C);
		const uint32_t sig = readFromFileOffset<uint32_t>(pe_header_offset);
		if (sig != ('P' | ('E' << 8)))
		{
			throw std::domain_error(std::string("Unexpected PE sig: ").append(std::string((char*)&sig, 4)));
		}
	}

	WindowsExecutableFile::WindowsExecutableFile(const std::string& path)
		: WindowsExecutableFile(std::fstream(path, std::ios::in | std::ios::out | std::ios::binary))
	{
	}

#define COFF_BASE (std::streamoff)pe_header_offset + 4
#define COFF_MACHINE         COFF_BASE
#define COFF_NUMSECTIONS     2 + COFF_MACHINE
#define COFF_TIMESTAMP       2 + COFF_NUMSECTIONS
#define COFF_SYMTBLPTR       4 + COFF_TIMESTAMP
#define COFF_NUMSYM          4 + COFF_SYMTBLPTR
#define COFF_OPTHEADERSIZE   4 + COFF_NUMSYM
#define COFF_CHARACTERISTICS 2 + COFF_OPTHEADERSIZE
#define COFF_END             2 + COFF_CHARACTERISTICS

	uint16_t WindowsExecutableFile::getCompiledForMachine()
	{
		return readFromFileOffset<uint16_t>(COFF_MACHINE);
	}

	uint16_t WindowsExecutableFile::getNumberOfSections()
	{
		return readFromFileOffset<uint16_t>(COFF_NUMSECTIONS);
	}

	uint32_t WindowsExecutableFile::getTimestamp()
	{
		return readFromFileOffset<uint32_t>(COFF_TIMESTAMP);
	}

	uint32_t WindowsExecutableFile::getNumberOfSymbols()
	{
		return readFromFileOffset<uint32_t>(COFF_NUMSYM);
	}

	uint16_t WindowsExecutableFile::getSizeOfOptionalHeader()
	{
		return readFromFileOffset<uint16_t>(COFF_OPTHEADERSIZE);
	}

	uint16_t WindowsExecutableFile::getCharacteristics()
	{
		return readFromFileOffset<uint16_t>(COFF_CHARACTERISTICS);
	}

#define SEC_BASE    getSectionOffsetInFile(section)
#define SEC_NAME    SEC_BASE
#define SEC_MISC    8 + SEC_NAME
#define SEC_VADDR   4 + SEC_MISC
#define SEC_SIZE    4 + SEC_VADDR
#define SEC_DATAOFF 4 + SEC_SIZE

	std::streamoff WindowsExecutableFile::getSectionOffsetInFile(uint16_t section)
	{
		return COFF_END + getSizeOfOptionalHeader() + (0x28 * (std::streamoff)section);
	}

	std::string WindowsExecutableFile::getSectionName(uint16_t section)
	{
		char name[8];
		file.seekg(SEC_BASE);
		file.read(name, 8);
		return std::string(name, name[7] == '\0' ? strlen(name) : 8);
	}

	void WindowsExecutableFile::setSectionName(uint16_t section, const std::string& name)
	{
		if (name.size() > 8)
		{
			throw std::overflow_error("Section name can't use more than 8 bytes");
		}
		const std::streamoff off = SEC_BASE;
		file.seekg(off);
		file.write("\0\0\0\0\0\0\0\0", 8);
		file.seekg(off);
		file.write(name.data(), name.size());
	}

	uint32_t WindowsExecutableFile::getSectionVirtualAddress(uint16_t section)
	{
		return readFromFileOffset<uint32_t>(SEC_VADDR);
	}

	uint32_t WindowsExecutableFile::getSectionDataSize(uint16_t section)
	{
		return readFromFileOffset<uint32_t>(SEC_SIZE);
	}

	uint32_t WindowsExecutableFile::getSectionDataOffsetInFile(uint16_t section)
	{
		return readFromFileOffset<uint32_t>(SEC_DATAOFF);
	}

	std::vector<uint8_t> WindowsExecutableFile::getSectionData(uint16_t section)
	{
		std::vector<uint8_t> data(getSectionDataSize(section));
		file.seekg(getSectionDataOffsetInFile(section));
		file.read((char*)data.data(), data.size());
		return data;
	}
}
