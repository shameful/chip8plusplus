#include "CPU.h"
#include "Memory.h"
#include "Display.h"
#include <iostream>
#include <bitset>


int main()
{
	std::cout << "nothing implemented yet\n";
	Mem::Chip8Mem mem;
	Display::Chip8Display display;
	CPU::Chip8CPU cpu;
	cpu.Set_Memory_Device(mem);
	cpu.Set_Display(display);
	cpu.Load_Sprites();
	cpu.Execute_Step();
	std::uint16_t addr = 0;
	std::uint8_t value = 0;
	for (std::uint16_t i = 0; i < 0x1000; i++)
	{
		addr = i;
		value = 0;
		value = mem.read(i);
		std::bitset<8> bits(value);
		if (value != 0xAA)
		{
			std::cout << std::hex << addr;
			std::cout << " -> ";
			std::cout << unsigned(value);
			std::cout << " = ";
			std::cout << bits;
			std::cout << "\n";
		}
	}
}