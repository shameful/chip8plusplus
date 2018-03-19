#include "Display.h"

namespace Display
{

	Chip8Display::Chip8Display()
	{
		Clear();
	}

	Chip8Display::~Chip8Display()
	{

	}

	void Chip8Display::Clear()
	{
		framebuffer.fill(0b00000000);
	}

	std::uint8_t Display::Chip8Display::DrawSprite(std::uint8_t x, std::uint8_t y, std::array<std::uint8_t, 15> sprite, std::uint8_t length)
	{
		if (x > 63 || y > 31 || length > 15) { return 0; } //don't attempt to draw if coords are out of bounds
		bool collision_flag = false;
		unsigned int intermed_x = 0;
		unsigned int intermed_y = 0;
		unsigned int resolved_coord = 0;
		unsigned int x_misalignment = 0;
		std::uint8_t sprite_part_a = 0x00;
		std::uint8_t sprite_part_b = 0x00;

		for (size_t i = 0; i < length; i++)
		{
			if ((x % 8) != 0) {
				//not aligned to byte array. will split sprite in two and write each part to neighboring framebuffer bytes
				x_misalignment = x % 8; //amount of bit misalignment
				intermed_x = x / 8;
				intermed_y = (y + i) % 32; //wrap around if going off screen
				sprite_part_a = sprite[i] >> x_misalignment;
				sprite_part_b = sprite[i] << (8 - x_misalignment);
				collision_flag = collision_flag | (sprite_part_a & framebuffer[intermed_x + intermed_y * 8]);
				collision_flag = collision_flag | (sprite_part_b & framebuffer[((intermed_x + 1) % 8) + intermed_y * 8]);
				framebuffer[intermed_x + intermed_y * 8] = framebuffer[intermed_x + intermed_y * 8] ^ sprite_part_a;
				framebuffer[((intermed_x + 1) % 8) + intermed_y * 8] = framebuffer[((intermed_x + 1) % 8) + intermed_y * 8] ^ sprite_part_b;
			}
			else
			{
				//aligned easy draw
				intermed_x = x / 8;
				intermed_y = (y + i) % 32; //wrap around if going off screen
				resolved_coord = intermed_x + intermed_y * 8;
				collision_flag = collision_flag | (sprite[i] & framebuffer[resolved_coord]);
				framebuffer[resolved_coord] = framebuffer[resolved_coord] ^ sprite[i];
			}
		}
		return collision_flag;
	}

	std::uint8_t Display::Chip8Display::read(std::uint16_t addr)
	{
		if (addr < size_framebuffer) { return framebuffer[addr]; } //bounds check
		return 0x00;
	}

	void Display::Chip8Display::write(std::uint16_t addr, std::uint8_t data)
	{
		if (addr < size_framebuffer) { framebuffer[addr] = data; }
	}

	
}