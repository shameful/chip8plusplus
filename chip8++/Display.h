#pragma once
#include <array>

namespace Display
{
	const unsigned int size_framebuffer = 0x100;

	class Chip8Display
	{
		
		std::array<std::uint8_t, size_framebuffer> framebuffer; //64x32 pixels

	public:

		bool flag_dirty; //true when the framebuffer has changed. display handler must manually set this false
		Chip8Display();
		~Chip8Display();

		//clear the display
		void Clear();

		std::uint8_t DrawSprite(std::uint8_t x, std::uint8_t y, std::array<std::uint8_t, 15> sprite, std::uint8_t length);
		std::uint8_t read(std::uint16_t addr);
		void write(std::uint16_t addr, std::uint8_t data);

	};
}