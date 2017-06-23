#pragma once
#include <array>

namespace Mem
{
	const std::uint8_t BIT_PATTERN = 0xAA;

	class Chip8Mem {

		std::array<std::uint8_t, 0x1000> memory;
		//mapping
		//0x000 - 0x1FF reserved for interpreter
		//0x200 - 0xFFF(?) program space

	public:
		std::uint8_t read(uint16_t addr)
		{
			std::uint8_t data = BIT_PATTERN;
			if (addr < 0x1000) { data = memory[addr]; } //bounds check
			return data;
		}

		void write(uint16_t addr, uint8_t data)
		{
			if (addr < 0x1000) { memory[addr] = data; } //bounds check
		}

		Chip8Mem()
		{
			memory.fill(BIT_PATTERN);
		}
		~Chip8Mem() {}
	};
}