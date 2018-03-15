#include "CPU.h"
#include "Memory.h"
#include "Display.h"
#include <iostream>
#include <bitset>


int main()
{
	std::cout << "not fully implemented\n";
	Mem::Chip8Mem mem;
	Display::Chip8Display display;
	CPU::Chip8CPU cpu;
	cpu.Set_Memory_Device(mem);
	cpu.Set_Display(display);
	cpu.Load_Sprites();

	mem.write(0x200, 0x60); mem.write(0x201, 0x0F); //0x600F - load 0x0F into V0
	mem.write(0x202, 0xF0); mem.write(0x203, 0x29); //0xF029 - load sprite for character in V0 into I
	mem.write(0x204, 0x60); mem.write(0x205, 0x00); //0x6000 - load 0x00 into V0
	mem.write(0x206, 0x61); mem.write(0x207, 0x00); //0x6100 - load 0x00 into V1
	mem.write(0x208, 0xD0); mem.write(0x209, 0x15); //0xD015 - draw a 5 row sprite at coords (V0, V1)

	//run the instructions
	for (size_t i = 0; i < 5; i++)
	{
		cpu.Execute_Step();
	} 
	
	//print the framebuffer
	for (size_t i = 0; i < 32; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			std::bitset<8> bits(display.read(i * 8 + j));
			std::cout << bits;
		}
		std::cout << "\n";
	}
	
}