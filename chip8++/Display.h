#pragma once

namespace Display
{
	class Chip8Display
	{

	public:

		Chip8Display();
		~Chip8Display();

		//clear the display
		void Clear();

		std::uint8_t DrawSprite(std::uint8_t x, std::uint8_t y, std::array<std::uint8_t, 15> sprite, std::uint8_t length);

	};
}