#pragma once
#include <array>

namespace Mem
{
	
	class Chip8Mem
	{

		std::array<std::uint8_t, 0x1000> memory;
		//mapping
		//0x000 - 0x1FF reserved for interpreter
		//0x200 - 0xFFF(?) program space

		bool sprites_loaded;

	public:
		
		std::uint8_t read(uint16_t addr);
		void write(uint16_t addr, uint8_t data);
		bool checkSpritesLoaded();
		void setSpritesLoaded(bool loaded);
		Chip8Mem();
		~Chip8Mem();
	};

}
