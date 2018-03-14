#pragma once
#include <array>

namespace Mem
{
	
	class Chip8Mem {

	public:
		std::uint8_t read(uint16_t addr);
		void write(uint16_t addr, uint8_t data);
		Chip8Mem();
		~Chip8Mem();
	};

}
