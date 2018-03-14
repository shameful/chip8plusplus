#include "../chip8++/Display.cpp"
#include <iostream>
#include <bitset>

int main()
{
	//verify draw methods work
	class DisplayTester : public Display::Chip8Display
	{
	public:
		void printFrame()
		{
			for (size_t i = 0; i < 32; i++)
			{
				for (size_t j = 0; j < 8; j++)
				{
					//j is x-coord, i is y-coord. 8 bytes per row, 32 rows
					std::bitset<8> frame_byte(framebuffer[j + i * 8]);
					std::cout << frame_byte;
				}
				std::cout << "\n";
			}
		}
	}frameTest;

	std::array<std::uint8_t, 15> test_sprite;
	test_sprite.fill(0b11100111);
	frameTest.DrawSprite(60, 28, test_sprite, 15);
	frameTest.printFrame();
	frameTest.Clear();
	std::cout << "\n";
	frameTest.printFrame();
	return 0;
}
