#include "CPU.h"
#include "Memory.h"
#include "Display.h"
#include <iostream>
#include <bitset>
#include <string>


int main()
{
	std::cout << "not fully implemented\n";
	Mem::Chip8Mem mem;
	Display::Chip8Display display;
	CPU::Chip8CPU cpu;
	cpu.Set_Memory_Device(mem);
	cpu.Set_Display(display);
	cpu.Load_Sprites();

	


	//run the instructions
	for (size_t i = 0; i < 1; i++)
	{
		cpu.Execute_Step();
	} 
	
	//print the framebuffer
	for (size_t i = 0; i < 32; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			std::string eight_pixels = "";
			std::uint8_t bits(display.read(i * 8 + j));
			if ((bits & 0b10000000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b01000000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00100000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00010000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00001000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00000100) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00000010) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00000001) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			std::cout << eight_pixels;
		}
		std::cout << "\n";
	}
	
}