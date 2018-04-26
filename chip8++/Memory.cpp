#include <array>
#include "Memory.h"

namespace Mem
{
	const std::uint8_t BIT_PATTERN = 0xAA;

	std::uint8_t Chip8Mem::read(uint16_t addr)
	{
		std::uint8_t data = BIT_PATTERN;
		if (addr < 0x1000) { data = memory[addr]; } //bounds check
		return data;
	}

	void Chip8Mem::write(uint16_t addr, uint8_t data)
	{
		if (addr < 0x1000) { memory[addr] = data; } //bounds check
	}

	bool Chip8Mem::checkSpritesLoaded()
	{
		return sprites_loaded;
	}

	void Chip8Mem::setSpritesLoaded(bool loaded)
	{
		sprites_loaded = loaded;
	}
	
	Chip8Mem::Chip8Mem()
	{
		memory.fill(BIT_PATTERN);
		sprites_loaded = false;
	}

	Chip8Mem::~Chip8Mem() {}
}
